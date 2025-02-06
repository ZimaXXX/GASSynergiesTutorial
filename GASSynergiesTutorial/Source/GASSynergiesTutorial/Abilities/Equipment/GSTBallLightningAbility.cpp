#include "GSTBallLightningAbility.h"
#include "AbilitySystemComponent.h"
#include "GASSynergiesTutorial/Actors/GSTBallLightningActor.h"
#include "GASSynergiesTutorial/Core/GSTCharacter.h"

UGSTBallLightningAbility::UGSTBallLightningAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGSTBallLightningAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayTagContainer* SourceTags,
                                                  const FGameplayTagContainer* TargetTags,
                                                  FGameplayTagContainer* OptionalRelevantTags) const
{
    return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGSTBallLightningAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(ActorInfo->OwnerActor);
    if (!Skimmer || !BallLightningClass)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    FVector SpawnLocation = Skimmer->GetActorLocation() + FVector(0, 0, 50.0f);

    AGSTBallLightningActor* BallLightning = GetWorld()->SpawnActor<AGSTBallLightningActor>(
        BallLightningClass, SpawnLocation, FRotator::ZeroRotator);

    if (BallLightning)
    {
        BallLightning->InitializeBallLightning(Skimmer, BallLightningDuration, OrbitRadius, OrbitSpeed);
        BallLightning->OnBallLightningDestroyed.AddDynamic(this, &UGSTBallLightningAbility::OnBallLightningDestroyed);
        ActiveBallLightnings.Add(BallLightning);
    }
}

void UGSTBallLightningAbility::OnBallLightningDestroyed(AGSTBallLightningActor* BallLightning)
{
    if (ActiveBallLightnings.Contains(BallLightning))
    {
        ActiveBallLightnings.Remove(BallLightning);

        if (ActiveBallLightnings.Num() == 0)
        {
            EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
        }
    }
}

void UGSTBallLightningAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Warning, TEXT("Ball Lightning Ability failed to commit!"));
    }
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
