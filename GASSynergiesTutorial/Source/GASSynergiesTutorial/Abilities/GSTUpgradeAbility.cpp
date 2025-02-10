#include "GSTUpgradeAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

UGSTUpgradeAbility::UGSTUpgradeAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // This ability activates automatically when this tag is applied
   // ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Upgrade.Active")));
}

void UGSTUpgradeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    // if (!ASC || !UpgradeEffect)
    // {
    //     EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    //     return;
    // }

    // Apply the Upgrade Effect
    FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(UpgradeEffect, 1.0f, ASC->MakeEffectContext());
    if (EffectSpec.IsValid())
    {
        UpgradeEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
    }
}

void UGSTUpgradeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                    const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    bool bReplicateEndAbility,
                                    bool bWasCancelled)
{
    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC && UpgradeEffectHandle.IsValid())
    {
        ASC->RemoveActiveGameplayEffect(UpgradeEffectHandle);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
