// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GSTEquipmentAbility.generated.h"

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
	/** Checks if the skimmer is over a surface with a specific GameplayTag */
	bool IsSkimmerOverMaterial(FGameplayTag MaterialTag);
};
