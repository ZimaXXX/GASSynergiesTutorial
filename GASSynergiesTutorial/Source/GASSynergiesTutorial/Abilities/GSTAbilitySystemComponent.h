// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GSTAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityEndedBP, UGameplayAbility*, GameplayAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityActivatedBP, UGameplayAbility*, GameplayAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilityChangedBP, UGameplayAbility*, GameplayAbility, bool, bActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilitySystemInitialzedDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASSYNERGIESTUTORIAL_API UGSTAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void BroadcastAbilityEndedCallbacksBP(UGameplayAbility* GameplayAbility) const;
	void BroadcastOnAbilityActivatedBP(UGameplayAbility* GameplayAbility) const;
	void InitializationCompleted();
	// Sets default values for this component's properties
	UGSTAbilitySystemComponent();

	UPROPERTY(BlueprintAssignable, Category= "Ability")
	FAbilityEndedBP BP_AbilityEndedCallback;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FAbilityActivatedBP BP_AbilityActivatedCallback;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FAbilityChangedBP BP_AbilityChangedCallback;

	UPROPERTY(BlueprintReadOnly, Category="Ability")
	bool bIsInitialized = false;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FAbilitySystemInitialzedDelegate OnAbilitySystemInitializedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeAbilities();
};
