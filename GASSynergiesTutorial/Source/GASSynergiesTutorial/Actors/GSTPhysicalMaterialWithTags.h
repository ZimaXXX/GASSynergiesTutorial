// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GSTPhysicalMaterialWithTags.generated.h"

/**
 * 
 */
UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTPhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
    /** Tags defining surface properties (e.g., "Material.Crystal", "Material.Sand") */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
    FGameplayTagContainer MaterialTags;
};
