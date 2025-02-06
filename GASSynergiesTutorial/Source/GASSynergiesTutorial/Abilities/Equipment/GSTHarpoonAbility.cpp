#include "GSTHarpoonAbility.h"
#include "AbilitySystemComponent.h"
#include "GASSynergiesTutorial/Core/GSTCharacter.h"
#include "GASSynergiesTutorial/Projectiles/GSTHarpoonProjectile.h"

UGSTHarpoonAbility::UGSTHarpoonAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGSTHarpoonAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayTagContainer* SourceTags,
                                            const FGameplayTagContainer* TargetTags,
                                            FGameplayTagContainer* OptionalRelevantTags) const
{
    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(ActorInfo->OwnerActor);
    if (!Skimmer)
    {
        return false;
    }

    if (!HarpoonProjectileClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Harpoon Ability Failed: HarpoonProjectileClass is not set!"));
        return false;
    }

    return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGSTHarpoonAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{


    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(ActorInfo->OwnerActor);
    if (!Skimmer)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    FVector SpawnLocation = Skimmer->GetActorLocation() + Skimmer->GetActorForwardVector() * 100.0f;
    FRotator SpawnRotation = Skimmer->GetActorRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Skimmer;
    SpawnParams.Instigator = Skimmer;

    AGSTHarpoonProjectile* Harpoon = GetWorld()->SpawnActor<AGSTHarpoonProjectile>(
        HarpoonProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (Harpoon)
    {
        Harpoon->InitializeProjectile(Skimmer, HarpoonSpeed, HarpoonRange, PullVelocityMultiplier);
    }
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGSTHarpoonAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Warning, TEXT("Harpoon Ability failed to commit!"));
    }
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
