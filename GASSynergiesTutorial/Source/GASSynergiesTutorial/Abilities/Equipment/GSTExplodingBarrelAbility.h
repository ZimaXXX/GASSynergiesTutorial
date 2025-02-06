#pragma once

#include "CoreMinimal.h"
#include "GASSynergiesTutorial/Abilities/GSTEquipmentAbility.h"
#include "GSTExplodingBarrelAbility.generated.h"

class UInputAction;
class AGSTExplodingBarrelActor;

UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTExplodingBarrelAbility : public UGSTEquipmentAbility
{
	GENERATED_BODY()

public:
	UGSTExplodingBarrelAbility();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
									const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayTagContainer* SourceTags,
									const FGameplayTagContainer* TargetTags,
									FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void SetupAbilityInput();
public:
	UFUNCTION(BlueprintCallable, Category = "Exploding Barrel")
	void SpawnBarrel();
private:
	/** The Exploding Barrel actor class */
	UPROPERTY(EditDefaultsOnly, Category = "Exploding Barrel")
	TSubclassOf<AGSTExplodingBarrelActor> ExplodingBarrelClass;

	/** Maximum number of active barrels */
	UPROPERTY(EditDefaultsOnly, Category = "Exploding Barrel")
	int32 MaxActiveBarrels = 3;

	/** Stores active barrels */
	TArray<AGSTExplodingBarrelActor*> ActiveBarrels;

	int32 SpawnedBarrels = 0;

	/** Input Action for spawning barrels */
	UPROPERTY(EditDefaultsOnly, Category = "Exploding Barrel")
	UInputAction* SpawnBarrelAction;
	


	/** Callback for when a barrel explodes */
	UFUNCTION()
	void OnBarrelExploded(AGSTExplodingBarrelActor* Barrel);
};
