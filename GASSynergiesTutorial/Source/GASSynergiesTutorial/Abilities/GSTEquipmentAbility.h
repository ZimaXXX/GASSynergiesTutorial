﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GSTEquipmentAbility.generated.h"

class UGSTEquipmentAttributeSet;
/**
 * 
 */
UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTEquipmentAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGSTEquipmentAbility();

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY()
	UAbilitySystemComponent* OwnerASC;

	UPROPERTY()
	const UGSTEquipmentAttributeSet* OwnerAttributes;
};
