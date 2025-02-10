#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "GameFramework/Actor.h"
#include "GSTHarpoonProjectile.generated.h"

class UGSTEquipmentAttributeSet;
class UGameplayEffect;
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
	UAbilitySystemComponent* OwnerASC;
	const UGSTEquipmentAttributeSet* OwnerAttributes;

	/** Checks if the hit surface has the Material.Rock tag */
	bool IsValidHarpoonSurface(const FHitResult& Hit) const;
	
	/** The GameplayEffect to apply for damage */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Damage amount applied when harpoon hits an enemy */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float BaseDamage = 10.0f;

	void ApplyDamageToEnemy(AActor* HitEnemy);
};
