#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "GASSynergiesTutorial/Projectiles/GSTProjectileBase.h"
#include "GSTExplodingBarrelActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBarrelExploded, AGSTExplodingBarrelActor*, Barrel);

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class GASSYNERGIESTUTORIAL_API AGSTExplodingBarrelActor : public AGSTProjectileBase
{
	GENERATED_BODY()
    
public:    
	AGSTExplodingBarrelActor();

	void InitializeBarrel(AActor* InOwner);

	/** Delegate triggered when the barrel explodes */
	FOnBarrelExploded OnBarrelExploded;

protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, bool bSelfMoved,
						   FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
						   const FHitResult& Hit) override;
	
	virtual float GetDamageFromAttribute() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ExplosionForce;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	FGameplayTagContainer NonExplosiveTags;

	UPROPERTY()
	AActor* OwnerSkimmer;

	float ExplosionRadius = 500.0f;
	float ExplosionDamage = 50.0f;

	void Explode();
	
	bool ShouldIgnoreMaterial(const FHitResult& Hit) const;
};
