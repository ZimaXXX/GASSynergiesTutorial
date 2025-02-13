#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GASSynergiesTutorial/Abilities/GSTEquipmentAbility.h"
#include "GASSynergiesTutorial/Abilities/GSTUpgradeAbility.h"
#include "GSTEquipmentDataAsset.generated.h"

/**
 * Data Asset that defines default Equipment and Upgrade Abilities for a character
 */
UCLASS(BlueprintType)
class GASSYNERGIESTUTORIAL_API UGSTEquipmentDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Default Equipment Abilities that should be granted on character spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment Abilities")
	TArray<TSubclassOf<UGSTEquipmentAbility>> DefaultEquipmentAbilities;

	/** Default Upgrade Abilities that should be unlocked on character spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade Abilities")
	TArray<TSubclassOf<UGSTUpgradeAbility>> DefaultUpgradeAbilities;
};
