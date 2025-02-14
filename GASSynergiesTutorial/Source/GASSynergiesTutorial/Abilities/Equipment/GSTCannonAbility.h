#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASSynergiesTutorial/Abilities/GSTEquipmentAbility.h"
#include "GSTCannonAbility.generated.h"

class AGSTCannonProjectile;

UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTCannonAbility : public UGSTEquipmentAbility
{
    GENERATED_BODY()

public:
    UGSTCannonAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
                            const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo,
                            bool bReplicateEndAbility,
                            bool bWasCancelled) override;

private:
    /** Timer handle for controlling fire rate */
    FTimerHandle FireTimerHandle;

    /** Timer handle for continuously looking for enemies */
    FTimerHandle EnemyLookupTimerHandle;

    /** Maximum range of the cannon */
    UPROPERTY(EditDefaultsOnly, Category = "Cannon")
    float MaxRange = 1500.0f;
    
    /** Maximum allowed horizontal deviation (e.g., 60° means 30° left and right) */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float AllowedAimAngle = 30.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float LookUpRate = 0.05f;

    /** Fires a projectile at the determined target */
    void FireCannon(AActor* TargetActor);

    /** Called to check if we can fire */
    void CheckIfCanFire();

    /** Continuously looks for enemies */
    void LookForEnemies();

    /** Finds the closest valid enemy within the allowed angle range */
    AActor* FindClosestValidEnemy();

    /** Returns whether the cannon is in cooldown */
    bool IsInCooldown();
    
    /** Draws debug lines to visualize valid shooting angles */
    void DrawDebugAimAssist();
};
