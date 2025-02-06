#pragma once

#include "CoreMinimal.h"
#include "GASSynergiesTutorial/Abilities/GSTEquipmentAbility.h"
#include "GSTJawsAbility.generated.h"

UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTJawsAbility : public UGSTEquipmentAbility
{
	GENERATED_BODY()

public:
	UGSTJawsAbility();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const override;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
								 const FGameplayAbilityActorInfo* ActorInfo, 
								 const FGameplayAbilityActivationInfo ActivationInfo, 
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, 
							const FGameplayAbilityActorInfo* ActorInfo, 
							const FGameplayAbilityActivationInfo ActivationInfo, 
							bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	/** Effect applied when burrowing (e.g., invulnerability) */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> BurrowEffect;

	/** Effect handle for tracking applied effects */
	FActiveGameplayEffectHandle ActiveEffectHandle;

	/** Timer handle to track when burrow should end */
	FTimerHandle BurrowTimerHandle;

	/** Ends burrow when the timer expires */
	void AutoEndBurrow();

};
