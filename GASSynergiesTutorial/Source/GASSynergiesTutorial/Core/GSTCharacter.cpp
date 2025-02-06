// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSTCharacter.h"

#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "GASSynergiesTutorial/GASSynergiesTutorialProjectile.h"
#include "GASSynergiesTutorial/Abilities/GSTAbilitySystemComponent.h"
#include "GASSynergiesTutorial/Actors/GSTPhysicalMaterialWithTags.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName AGSTCharacter::MoveForwardBinding("MoveForward");
const FName AGSTCharacter::MoveRightBinding("MoveRight");
const FName AGSTCharacter::FireForwardBinding("FireForward");
const FName AGSTCharacter::FireRightBinding("FireRight");

AGSTCharacter::AGSTCharacter()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	// Create GAS components
	AbilitySystemComponent = CreateDefaultSubobject<UGSTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGSTEquipmentAttributeSet>(TEXT("AttributeSet"));

	// Ensure the AbilitySystemComponent replicates
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void AGSTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Grant default abilities
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
		}

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->InitStats(UGSTEquipmentAttributeSet::StaticClass(), nullptr);
	}
}

bool AGSTCharacter::IsSkimmerOverMaterial(FGameplayTag MaterialTag) const
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + FVector(0, 0, -500); // Raycast 500 units downward

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bReturnPhysicalMaterial = true; // Ensure we get the PhysMaterial

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	if (bHit && HitResult.PhysMaterial.IsValid())
	{
		UGSTPhysicalMaterialWithTags* PhysMat = Cast<UGSTPhysicalMaterialWithTags>(HitResult.PhysMaterial.Get());
		return PhysMat && PhysMat->MaterialTags.HasTag(MaterialTag);
	}

	return false;
}
UAbilitySystemComponent* AGSTCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGSTCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AGSTCharacter::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
	
	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void AGSTCharacter::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// spawn the projectile
				World->SpawnActor<AGASSynergiesTutorialProjectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AGSTCharacter::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AGSTCharacter::ShotTimerExpired()
{
	bCanFire = true;
}

void AGSTCharacter::OnBurrowStarted()
{
	SetActorEnableCollision(false);
	OriginalZ = GetActorLocation().Z;
	BurrowedZ = OriginalZ - 200.0f; // Move down 200 units
}

void AGSTCharacter::StartBurrowing()
{
    OnBurrowStarted();

	// Smoothly move down
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AGSTCharacter::MoveToBurrow, 0.01f, true);
}

void AGSTCharacter::MoveToBurrow()
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FVector(CurrentLocation.X, CurrentLocation.Y, 
								  FMath::FInterpTo(CurrentLocation.Z, BurrowedZ, GetWorld()->GetDeltaSeconds(), 5.0f));

	SetActorLocation(NewLocation);

	if (FMath::Abs(CurrentLocation.Z - BurrowedZ) < 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	}
}

void AGSTCharacter::EndBurrowing()
{
	if(!IsBuried())
	{
		return;
	}
	// Smoothly move back up
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AGSTCharacter::MoveToSurface, 0.01f, true);
}

void AGSTCharacter::OnBurrowFinished()
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FVector(CurrentLocation.X, CurrentLocation.Y, OriginalZ);
	SetActorLocation(NewLocation);	
	SetActorEnableCollision(true);
}

bool AGSTCharacter::IsBuried() const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Buried"));
	}
	return false;
}

void AGSTCharacter::MoveToSurface()
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FVector(CurrentLocation.X, CurrentLocation.Y, 
								  FMath::FInterpTo(CurrentLocation.Z, OriginalZ, GetWorld()->GetDeltaSeconds(), 5.0f));

	SetActorLocation(NewLocation);

	if (FMath::Abs(CurrentLocation.Z - OriginalZ) < 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
		OnBurrowFinished();
	}
}

