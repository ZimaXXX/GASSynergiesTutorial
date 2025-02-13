// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GASSynergiesTutorial/Abilities/GSTUpgradeAbility.h"
#include "GSTEquipmentManagerComponent.generated.h"


class UGSTEquipmentAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASSYNERGIESTUTORIAL_API UGSTEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGSTEquipmentManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool ActivateAbilityByTag(FGameplayTag InTag);

	UFUNCTION(BlueprintCallable)
	TArray<UGSTUpgradeAbility*> GetUpgradeAbilities();

	UFUNCTION(BlueprintCallable)
	TArray<UGSTEquipmentAbility*> GetEquipmentAbilities();

	UFUNCTION(BlueprintCallable)
	UGameplayAbility* GetAbilityByTag(FGameplayTag InTag);

	UFUNCTION(BlueprintCallable)
	bool IsAbilityActivatedByTag(FGameplayTag InTag, UGameplayAbility*& OutAbility);
};
