// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASSynergiesTutorialGameMode.h"
#include "GASSynergiesTutorialPawn.h"

AGASSynergiesTutorialGameMode::AGASSynergiesTutorialGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AGASSynergiesTutorialPawn::StaticClass();
}

