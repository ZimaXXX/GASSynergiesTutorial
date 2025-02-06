#include "GSTExplodingBarrelActor.h"

#include "GSTPhysicalMaterialWithTags.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/Actor.h"

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

void AGSTExplodingBarrelActor::Explode()
{
	ExplosionForce->FireImpulse();

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