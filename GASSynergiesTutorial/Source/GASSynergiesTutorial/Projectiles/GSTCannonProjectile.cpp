#include "GSTCannonProjectile.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GASSynergiesTutorial/Enemy/GSTEnemyPawn.h"

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

	// Ensure the projectile ignores the ship's collision
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
	// Ignore if the projectile hits the ship
	if (OtherActor == OwnerActor)
	{
		return;
	}
	
	AGSTEnemyPawn* Enemy = Cast<AGSTEnemyPawn>(OtherActor);
	if (Enemy && DamageEffectClass)
	{
		UAbilitySystemComponent* EnemyASC = Enemy->GetAbilitySystemComponent();
		if (EnemyASC)
		{
			// Create an effect spec
			FGameplayEffectSpecHandle DamageEffect = EnemyASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EnemyASC->MakeEffectContext());

			if (DamageEffect.IsValid())
			{
				// Apply dynamic damage value
				DamageEffect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Damage")), -DamageAmount);
				EnemyASC->ApplyGameplayEffectSpecToSelf(*DamageEffect.Data.Get());

				// Debug log
				UE_LOG(LogTemp, Warning, TEXT("Projectile applied BP_GE_ApplyDamage to enemy: %s | Damage: %f"), *Enemy->GetName(), -DamageAmount);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Cannon Projectile hit: %s"), *OtherActor->GetName());
	Destroy();
}