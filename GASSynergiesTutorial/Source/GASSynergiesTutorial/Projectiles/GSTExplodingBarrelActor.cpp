#include "GSTExplodingBarrelActor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/Actor.h"
#include "GASSynergiesTutorial/Actors/GSTPhysicalMaterialWithTags.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"
#include "GASSynergiesTutorial/Enemy/GSTEnemyPawn.h"

AGSTExplodingBarrelActor::AGSTExplodingBarrelActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName("BlockAll");
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionForce"));
	ExplosionForce->SetupAttachment(RootComponent);
	ExplosionForce->Radius = ExplosionRadius;
	ExplosionForce->ImpulseStrength = 3000.0f;
	ExplosionForce->bImpulseVelChange = true;
}

void AGSTExplodingBarrelActor::InitializeBarrel(AActor* InOwner)
{
	OwnerSkimmer = InOwner;
	
	FVector ImpulseDirection = FVector(FMath::RandRange(-1.0f, 1.0f), 
									   FMath::RandRange(-1.0f, 1.0f), 
									   0.5f).GetSafeNormal();
	MeshComponent->AddImpulse(ImpulseDirection * 500.0f, NAME_None, true);
}

void AGSTExplodingBarrelActor::NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, bool bSelfMoved,
										 FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
										 const FHitResult& Hit)
{
	if (!ShouldIgnoreMaterial(Hit))
	{
		Explode();
	}
}

float AGSTExplodingBarrelActor::GetDamageFromAttribute()
{
	return OwnerAttributes->GetExplodingBarrelDamage();
}

void AGSTExplodingBarrelActor::Explode()
{
	ExplosionForce->FireImpulse();
	// Define the start and end points for the trace (center of explosion)
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation; // Since it's a sphere trace, start and end are the same

	// Sphere trace parameters
	TArray<FHitResult> HitResults;
	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(ExplosionRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore the barrel itself

	// Perform the trace
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults, StartLocation, EndLocation, FQuat::Identity,
		ECC_Pawn, ExplosionSphere, QueryParams
	);

	if(bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if(AGSTEnemyPawn* Enemy = Cast<AGSTEnemyPawn>(Hit.GetActor()))
			{
				ApplyDamageToEnemy(Enemy);
			}
		}
	}

	// Debug visualization
	DrawDebugSphere(GetWorld(), StartLocation, ExplosionRadius, 16, FColor::Red, false, 2.0f);

	OnBarrelExploded.Broadcast(this);
	Destroy();
}


bool AGSTExplodingBarrelActor::ShouldIgnoreMaterial(const FHitResult& Hit) const
{
	if (Hit.PhysMaterial.IsValid())
	{
		UGSTPhysicalMaterialWithTags* PhysMat = Cast<UGSTPhysicalMaterialWithTags>(Hit.PhysMaterial.Get());
		if (PhysMat)
		{
			for (const FGameplayTag& Tag : NonExplosiveTags)
			{
				if (PhysMat->MaterialTags.HasTag(Tag))
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit a non-explosive material: %s"), *Tag.ToString());
					return true;
				}
			}
		}
	}
	return false;
}