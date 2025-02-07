#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GSTEquipmentAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTEquipmentAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGSTEquipmentAttributeSet();

	/** Fire rate of weapons */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData FireRate;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, FireRate)

	/** Base damage dealt */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, Damage)

	/** Ability cooldown time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData Cooldown;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, Cooldown)

	/** Damage Resistance: 0.0 (Full Damage), 1.0 (Full Immunity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData DamageResistance;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, DamageResistance)

	/** Maximum burrow time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData BurrowMaxTime;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, BurrowMaxTime)

};
