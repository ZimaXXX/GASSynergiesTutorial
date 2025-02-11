#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GSTUpgradeAbility.generated.h"

UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTUpgradeAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGSTUpgradeAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
                            const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo,
                            bool bReplicateEndAbility,
                            bool bWasCancelled) override;

private:
    /** The passive GameplayEffect applied when this upgrade is active */
    UPROPERTY(EditDefaultsOnly, Category = "Upgrade")
    TSubclassOf<UGameplayEffect> UpgradeEffect;

    /** Handle for the applied GameplayEffect */
    FActiveGameplayEffectHandle UpgradeEffectHandle;

public:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Upgrade")
    FName FriendlyName;
};
