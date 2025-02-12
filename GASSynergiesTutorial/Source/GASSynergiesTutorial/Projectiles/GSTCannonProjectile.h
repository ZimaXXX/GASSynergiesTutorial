#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GSTProjectileBase.h"
#include "GameFramework/Actor.h"
#include "GSTCannonProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GASSYNERGIESTUTORIAL_API AGSTCannonProjectile : public AGSTProjectileBase
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
	
	virtual float GetDamageFromAttribute() override;

private:
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY()
	AActor* OwnerActor;

	float MaxRange;
};
