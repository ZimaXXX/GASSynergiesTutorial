#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "GSTEnemyPawn.generated.h"

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);

class UWidgetComponent;
class UAbilitySystemComponent;
class UGSTEnemyAttributeSet;

UCLASS()
class GASSYNERGIESTUTORIAL_API AGSTEnemyPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGSTEnemyPawn();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const;

	/** Delegate for broadcasting health changes */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChangedDelegate;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	UGSTEnemyAttributeSet* EnemyAttributes;

	void HandleHealthChanged(const FOnAttributeChangeData& Data);
};
