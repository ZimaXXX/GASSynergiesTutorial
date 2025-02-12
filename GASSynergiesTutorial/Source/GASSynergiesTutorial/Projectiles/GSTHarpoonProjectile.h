#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GSTProjectileBase.h"

#include "GameFramework/Actor.h"
#include "GSTHarpoonProjectile.generated.h"

class UGSTEquipmentAttributeSet;
class UGameplayEffect;
class UCableComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Blueprintable, BlueprintType)
class GASSYNERGIESTUTORIAL_API AGSTHarpoonProjectile : public AGSTProjectileBase
{
	GENERATED_BODY()

public:    
	AGSTHarpoonProjectile();

	void InitializeProjectile(AActor* InOwner, float Speed, float InPullVelocityMultiplier);

	UFUNCTION(BlueprintImplementableEvent)
	FGameplayTagContainer ReceiveGetAllowedMaterialTags(int32 InHardnessLevel) const;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, bool bSelfMoved,
						   FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
						   const FHitResult& Hit) override;
	virtual float GetDamageFromAttribute() override;

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
	
	UPROPERTY(EditDefaultsOnly, Category = "Range")
	float BaseRange = 1500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Hardness")
	int32 BaseHardnessLevel = 1500.0f;
};
