// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASSynergiesTutorialGameMode.h"
#include "Core/GSTCharacter.h"

AGASSynergiesTutorialGameMode::AGASSynergiesTutorialGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AGSTCharacter::StaticClass();
}

