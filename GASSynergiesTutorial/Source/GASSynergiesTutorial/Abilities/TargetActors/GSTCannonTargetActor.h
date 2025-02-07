#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GSTCannonTargetActor.generated.h"

UCLASS()
class GASSYNERGIESTUTORIAL_API AGSTCannonTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AGSTCannonTargetActor();

	/** Starts continuous targeting */
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	/** Handles confirming the selected target */
	virtual void ConfirmTargetingAndContinue() override;

	/** Stops continuous scanning if needed */
	void StopScanning();

private:
	/** Maximum search range */
	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float MaxTargetingRange = 1500.0f;

	/** Maximum allowed horizontal deviation (e.g., 60° means 30° left and right) */
	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float AllowedAimAngle = 30.0f; 
	
	/** Timer handle for continuous scanning */
	FTimerHandle TargetScanTimer;

	/** Continuously scans for the closest valid enemy */
	void ScanForTarget();

	/** Finds the closest enemy that fits within the valid aim angle range */
	AActor* FindClosestValidEnemy();

	/** Draws debug lines to visualize valid shooting angles */
	void DrawDebugAimAssist();

};
