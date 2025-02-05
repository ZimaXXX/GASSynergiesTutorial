// Fill out your copyright notice in the Description page of Project Settings.


#include "GSTPlayerController.h"
#include "EnhancedInputComponent.h"
#include "GSTCharacter.h"
#include "GASSynergiesTutorial/Abilities/GSTAbilitySystemComponent.h"

void AGSTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Get character reference
	AGSTCharacter* GSTCharacter = Cast<AGSTCharacter>(GetPawn());
	if (!GSTCharacter || !GSTCharacter->GetAbilitySystemComponent()) return;

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComp) return;

	UGSTAbilitySystemComponent* ASC = Cast<UGSTAbilitySystemComponent>(GSTCharacter->GetAbilitySystemComponent());
	if (!ASC) return;

	// Bind ability activation to input
	//EnhancedInputComp->BindAction(/* YourActionMapping */, ETriggerEvent::Started, ASC, &UAbilitySystemComponent::AbilityLocalInputPressed);
}
