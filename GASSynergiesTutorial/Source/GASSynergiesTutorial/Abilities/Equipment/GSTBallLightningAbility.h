#pragma once

#include "CoreMinimal.h"
#include "GASSynergiesTutorial/Abilities/GSTEquipmentAbility.h"
#include "GSTBallLightningAbility.generated.h"

class AGSTBallLightningActor;

UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTBallLightningAbility : public UGSTEquipmentAbility
{
	GENERATED_BODY()

public:
	UGSTBallLightningAbility();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
									const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayTagContainer* SourceTags,
									const FGameplayTagContainer* TargetTags,
									FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	UFUNCTION()
	void OnBallLightningDestroyed(AGSTBallLightningActor* BallLightning);
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "BallLightning")
	float OrbitSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "BallLightning")
	float OrbitRadius = 200.f; 

	/** Stores the spawned ball lightning instances */
	TArray<AGSTBallLightningActor*> ActiveBallLightnings;
};
