#include "GSTSynergyAbility.h"
#include "AbilitySystemComponent.h"

UGSTSynergyAbility::UGSTSynergyAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGSTSynergyAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
										 const FGameplayAbilityActorInfo* ActorInfo, 
										 const FGameplayAbilityActivationInfo ActivationInfo, 
										 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (SynergyEffect)
		{
			FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(SynergyEffect, 1.0f, ASC->MakeEffectContext());
			ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		}
	}
}

void UGSTSynergyAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, 
									const FGameplayAbilityActorInfo* ActorInfo, 
									const FGameplayAbilityActivationInfo ActivationInfo, 
									bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (ActiveEffectHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
		}
	}
}