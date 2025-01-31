// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GSTSynergyAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTSynergyAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGSTSynergyAbility();

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
	UPROPERTY(EditDefaultsOnly, Category = "Synergy")
	TSubclassOf<UGameplayEffect> SynergyEffect;

	FActiveGameplayEffectHandle ActiveEffectHandle;
};
