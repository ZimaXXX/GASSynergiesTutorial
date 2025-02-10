#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "GSTCannonProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GASSYNERGIESTUTORIAL_API AGSTCannonProjectile : public AActor
{
	GENERATED_BODY()
    
public:    
	AGSTCannonProjectile();

	void InitializeProjectile(AActor* InOwner, float Range);

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
			   const FHitResult& Hit);


private:
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY()
	AActor* OwnerActor;

	float MaxRange;
	
	/** Damage amount applied on hit */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float DamageAmount = 25.0f;

	/** The GameplayEffect applied for damage */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
