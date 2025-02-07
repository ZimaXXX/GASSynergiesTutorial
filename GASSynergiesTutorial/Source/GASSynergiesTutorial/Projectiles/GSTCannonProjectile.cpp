#include "GSTCannonProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AGSTCannonProjectile::AGSTCannonProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(10.0f);
	CollisionComponent->SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AGSTCannonProjectile::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}


void AGSTCannonProjectile::InitializeProjectile(AActor* InOwner, float Range)
{
	OwnerActor = InOwner;
	MaxRange = Range;
	SetLifeSpan(Range / ProjectileMovement->MaxSpeed);

	// ✅ Ensure the projectile ignores the ship's collision
	if (OwnerActor)
	{
		CollisionComponent->IgnoreActorWhenMoving(OwnerActor, true);
		CollisionComponent->MoveIgnoreActors.Add(OwnerActor);
	}
}


void AGSTCannonProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, FVector NormalImpulse,
								 const FHitResult& Hit)
{
	// ✅ Ignore if the projectile hits the ship
	if (OtherActor == OwnerActor)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Cannon Projectile hit: %s"), *OtherActor->GetName());
	Destroy();
}