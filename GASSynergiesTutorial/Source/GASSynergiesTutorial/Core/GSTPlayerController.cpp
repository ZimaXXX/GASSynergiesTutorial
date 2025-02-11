// Fill out your copyright notice in the Description page of Project Settings.


#include "GSTPlayerController.h"

void AGSTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputData;
	InputData.SetHideCursorDuringCapture(false);
	SetInputMode(InputData);
}
