#pragma once

#include "CoreMinimal.h"
#include "GSTEquipmentAbility.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GSTCannonAbility.generated.h"

class AGSTCannonProjectile;
class AGSTCannonTargetActor;

UCLASS()
class GASSYNERGIESTUTORIAL_API UGSTCannonAbility : public UGSTEquipmentAbility
{
    GENERATED_BODY()

public:
    UGSTCannonAbility();

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

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
                            const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo,
                            bool bReplicateEndAbility,
                            bool bWasCancelled) override;

private:
    /** The Cannon projectile class */
    UPROPERTY(EditDefaultsOnly, Category = "Cannon")
    TSubclassOf<AGSTCannonProjectile> CannonProjectileClass;

    /** Targeting actor for finding closest enemy */
    UPROPERTY(EditDefaultsOnly, Category = "Cannon")
    TSubclassOf<AGSTCannonTargetActor> TargetActorClass;

    /** The time between shots */
    UPROPERTY(EditDefaultsOnly, Category = "Cannon")
    float FireRate = 0.5f;

    /** Maximum range of the cannon */
    UPROPERTY(EditDefaultsOnly, Category = "Cannon")
    float MaxRange = 1500.0f;

    /** Whether the cannon fires to the right (false = left) */
    bool bFireRight = true;

    /** Handles automatic firing */
    FTimerHandle FireTimerHandle;

    /** Handles the received target data */
    void OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& Data);

    /** Fires a projectile at the determined target */
    void FireCannon(AActor* TargetActor);
};
