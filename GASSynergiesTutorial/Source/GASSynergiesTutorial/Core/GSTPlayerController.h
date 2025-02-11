// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GSTPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GASSYNERGIESTUTORIAL_API AGSTPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
