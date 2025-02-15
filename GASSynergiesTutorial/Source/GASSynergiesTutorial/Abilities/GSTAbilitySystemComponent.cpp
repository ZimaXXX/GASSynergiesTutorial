﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GSTAbilitySystemComponent.h"


void UGSTAbilitySystemComponent::BroadcastAbilityEndedCallbacksBP(UGameplayAbility* GameplayAbility) const
{
	BP_AbilityEndedCallback.Broadcast(GameplayAbility);
	BP_AbilityChangedCallback.Broadcast(GameplayAbility, false);
}

void UGSTAbilitySystemComponent::BroadcastOnAbilityActivatedBP(UGameplayAbility* GameplayAbility) const
{
	BP_AbilityActivatedCallback.Broadcast(GameplayAbility);
	BP_AbilityChangedCallback.Broadcast(GameplayAbility, true);
}

void UGSTAbilitySystemComponent::InitializationCompleted()
{
	bIsInitialized = true;
	OnAbilitySystemInitializedDelegate.Broadcast();
}

// Sets default values for this component's properties
UGSTAbilitySystemComponent::UGSTAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	AbilityEndedCallbacks.AddUObject(this, &UGSTAbilitySystemComponent::BroadcastAbilityEndedCallbacksBP);
	AbilityActivatedCallbacks.AddUObject(this, &UGSTAbilitySystemComponent::BroadcastOnAbilityActivatedBP);
}


// Called when the game starts
void UGSTAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGSTAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGSTAbilitySystemComponent::InitializeAbilities()
{
	// Placeholder for initializing abilities later
}

