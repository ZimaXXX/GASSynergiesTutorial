// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSTCharacter.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/GSTEquipmentManagerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GASSynergiesTutorial/GASSynergiesTutorialProjectile.h"
#include "GASSynergiesTutorial/Abilities/GSTAbilitySystemComponent.h"
#include "GASSynergiesTutorial/Actors/GSTPhysicalMaterialWithTags.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"
#include "GASSynergiesTutorial/Data/GSTEquipmentDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

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
	MoveSpeed = 300.0f;
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
	
	EquipmentManager = CreateDefaultSubobject<UGSTEquipmentManagerComponent>(TEXT("EquipmentManager"));

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
	FloatingMovement->MaxSpeed = 1200.0f;
	
}

void AGSTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Grant default abilities
	if (HasAuthority() && AbilitySystemComponent)
	{
		GrantDefaultAbilities();
		AbilitySystemComponent->InitAbilityActorInfo(this, this);		
		AbilitySystemComponent->InitStats(UGSTEquipmentAttributeSet::StaticClass(), EquipmentAttributesDataTable);
	}
	AbilitySystemComponent->InitializationCompleted();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
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

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent || !DefaultMappingContext)
	{
		return;
	}

	// Iterate through all Input Actions inside the Mapping Context and bind them
	for (const FEnhancedActionKeyMapping& Mapping : DefaultMappingContext->GetMappings())
	{
		if (Mapping.Action->GetFName() == "IA_Move")
		{
			EnhancedInputComponent->BindAction(Mapping.Action, ETriggerEvent::Triggered, this, &AGSTCharacter::Move);
		}
	}
}

void AGSTCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	// Find movement direction
	const float ForwardValue = MoveVector.Y;
	const float RightValue = MoveVector.X;
	
	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	
	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * GetWorld()->GetDeltaSeconds();
	
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
}

void AGSTCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

void AGSTCharacter::GrantDefaultAbilities()
{
	if (!EquipmentDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("GSTCharacter: No EquipmentDataAsset assigned!"));
		return;
	}

	// Grant Equipment Abilities
	for (TSubclassOf<UGameplayAbility> AbilityClass : EquipmentDataAsset->DefaultEquipmentAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
	}

	// Unlock Upgrade Abilities
	for (TSubclassOf<UGameplayAbility> UpgradeAbilityClass : EquipmentDataAsset->DefaultUpgradeAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UpgradeAbilityClass, 1, INDEX_NONE, this));
	}
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

