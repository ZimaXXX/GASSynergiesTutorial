#pragma once

#include "CoreMinimal.h"
#include "GASSynergiesTutorial/Abilities/GSTEquipmentAbility.h"
#include "GSTHarpoonAbility.generated.h"

class AGSTHarpoonProjectile;

UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTHarpoonAbility : public UGSTEquipmentAbility
{
	GENERATED_BODY()

public:
	UGSTHarpoonAbility();

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

private:

	/** The initial speed of the harpoon */
	UPROPERTY(EditDefaultsOnly, Category = "Harpoon")
	float HarpoonSpeed = 3000.0f;

	/** The range of the harpoon */
	UPROPERTY(EditDefaultsOnly, Category = "Harpoon")
	float HarpoonRange = 1500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Harpoon")
	float PullVelocityMultiplier = 5.f;
};
