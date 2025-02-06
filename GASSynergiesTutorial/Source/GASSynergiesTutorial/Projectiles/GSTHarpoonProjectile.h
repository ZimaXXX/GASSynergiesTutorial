#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GSTHarpoonProjectile.generated.h"

class UCableComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Blueprintable, BlueprintType)
class GASSYNERGIESTUTORIAL_API AGSTHarpoonProjectile : public AActor
{
	GENERATED_BODY()
    
public:    
	AGSTHarpoonProjectile();

	void InitializeProjectile(AActor* InOwner, float Speed, float Range, float InPullVelocityMultiplier);

protected:
	virtual void BeginPlay() override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, bool bSelfMoved,
						   FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
						   const FHitResult& Hit) override;

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* SceneRootComponent;
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	UCableComponent* CableComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	TObjectPtr<AActor> OwnerSkimmer;

	float PullVelocityMultiplier = 1.f;

	/** Checks if the hit surface has the Material.Rock tag */
	bool IsValidHarpoonSurface(const FHitResult& Hit) const;
};
