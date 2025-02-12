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

	//General
	/** Damage Resistance: 0.0 (Full Damage), 1.0 (Full Immunity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData DamageResistance;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, DamageResistance)

	//Burrow
	/** Maximum burrow time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData BurrowMaxTime;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, BurrowMaxTime)
	
	/** Ability cooldown time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData BurrowCooldown;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, BurrowCooldown)

	//Harpoon
	/** Harpoon Damage */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData HarpoonDamage;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, HarpoonDamage);

	/** Harpoon Range */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData HarpoonRange;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, HarpoonRange);
	
	/** Harpoon Hardness */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData HarpoonHardness;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, HarpoonHardness);

	/** Ability cooldown time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData HarpoonCooldown;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, HarpoonCooldown)

	//Cannon
	/** Cannon Damage */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CannonDamage;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, CannonDamage);

	/** Cannon Fire Rate */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CannonFireRate;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, CannonFireRate);

	//Ball Ligtning
	/** Ball Lightning Damage */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData BallLightningDamage;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, BallLightningDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData BallLightningCount;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, BallLightningCount);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData BallLightningDuration;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, BallLightningDuration);

	/** Ability cooldown time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData BallLightningCooldown;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, BallLightningCooldown)

	//Exploding Barrel

	/** Exploding Barrel */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ExplodingBarrelDamage;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, ExplodingBarrelDamage);

	/** Exploding Barrel */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData ExplodingBarrelCooldown;
	ATTRIBUTE_ACCESSORS(UGSTEquipmentAttributeSet, ExplodingBarrelCooldown)

};
