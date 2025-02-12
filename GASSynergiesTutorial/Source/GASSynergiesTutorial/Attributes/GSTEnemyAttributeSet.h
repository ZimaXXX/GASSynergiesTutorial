#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GSTEquipmentAttributeSet.h"
#include "GSTEnemyAttributeSet.generated.h"

USTRUCT(BlueprintType)
struct GASSYNERGIESTUTORIAL_API FAttributeInitData : public FTableRowBase
{
	GENERATED_BODY()

	/** The attribute to initialize */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttribute Attribute;

	/** The default value of the attribute */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float DefaultValue = 0.0f;
};


UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGSTEnemyAttributeSet();

	/** Health Attribute */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGSTEnemyAttributeSet, Health);

	/** Max Health Attribute */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGSTEnemyAttributeSet, MaxHealth);

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
