// Fill out your copyright notice in the Description page of Project Settings.


#include "GSTEquipmentManagerComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GASSynergiesTutorial/Abilities/GSTAbilitySystemComponent.h"
#include "GASSynergiesTutorial/Abilities/GSTEquipmentAbility.h"


// Sets default values for this component's properties
UGSTEquipmentManagerComponent::UGSTEquipmentManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGSTEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGSTEquipmentManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UGSTEquipmentManagerComponent::ActivateAbilityByTag(FGameplayTag InTag)
{
	if(UGSTAbilitySystemComponent* ASC = Cast<UGSTAbilitySystemComponent>(GetOwner()->GetComponentByClass(UGSTAbilitySystemComponent::StaticClass())); ASC->bIsInitialized)
	{
		return ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(InTag));
	}
	return false;
}

TArray<UGSTUpgradeAbility*> UGSTEquipmentManagerComponent::GetUpgradeAbilities()
{
	TArray<UGSTUpgradeAbility*> UpgradeAbilities;
	if(UGSTAbilitySystemComponent* ASC = Cast<UGSTAbilitySystemComponent>(GetOwner()->GetComponentByClass(UGSTAbilitySystemComponent::StaticClass())); ASC->bIsInitialized)
	{
		TArray<FGameplayAbilitySpecHandle> Handles;
		ASC->GetAllAbilities(Handles);
		for(FGameplayAbilitySpecHandle Handle : Handles)
		{
			bool bIsInstance = false;
			if(UGameplayAbility* Ability =  const_cast<UGameplayAbility*>(UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(ASC, Handle, bIsInstance)); bIsInstance)
			{
				if(UGSTUpgradeAbility* UpgradeAbility = Cast<UGSTUpgradeAbility>(Ability))
				{
					UpgradeAbilities.Add(UpgradeAbility);
				}
			}
		}		
	}
	return UpgradeAbilities;
}

TArray<UGSTEquipmentAbility*> UGSTEquipmentManagerComponent::GetEquipmentAbilities()
{
	TArray<UGSTEquipmentAbility*> EquipmentAbilities;
	if(UGSTAbilitySystemComponent* ASC = Cast<UGSTAbilitySystemComponent>(GetOwner()->GetComponentByClass(UGSTAbilitySystemComponent::StaticClass())); ASC->bIsInitialized)
	{
		TArray<FGameplayAbilitySpecHandle> Handles;
		ASC->GetAllAbilities(Handles);
		for(FGameplayAbilitySpecHandle Handle : Handles)
		{
			bool bIsInstance = false;
			if(UGameplayAbility* Ability = const_cast<UGameplayAbility*>(UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(ASC, Handle, bIsInstance)); bIsInstance)
			{
				if(UGSTEquipmentAbility* EquipmentAbility = Cast<UGSTEquipmentAbility>(Ability))
				{
					EquipmentAbilities.Add(EquipmentAbility);
				}
			}
		}
		
	}
	return EquipmentAbilities;
}

UGameplayAbility* UGSTEquipmentManagerComponent::GetAbilityByTag(const FGameplayTag InTag)
{
	if(UGSTAbilitySystemComponent* ASC = Cast<UGSTAbilitySystemComponent>(GetOwner()->GetComponentByClass(UGSTAbilitySystemComponent::StaticClass())); ASC->bIsInitialized)
	{
		TArray<FGameplayAbilitySpecHandle> Handles;
		ASC->FindAllAbilitiesWithTags(Handles, FGameplayTagContainer(InTag));
		if(Handles.Num() > 0)
		{
			bool bIsInstance = false;
			return const_cast<UGameplayAbility*>(UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(ASC, Handles[0], bIsInstance));
		}		
	}
	return nullptr;
}

bool UGSTEquipmentManagerComponent::IsAbilityActivatedByTag(FGameplayTag InTag, UGameplayAbility*& OutAbility)
{
	OutAbility = GetAbilityByTag(InTag);
	if(OutAbility)
	{
		return OutAbility->IsActive();
	}
	return false;
}
