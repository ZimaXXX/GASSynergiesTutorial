#include "GSTExplodingBarrelAbility.h"
#include "AbilitySystemComponent.h"
#include "GASSynergiesTutorial/Core/GSTCharacter.h"
#include "GASSynergiesTutorial/Projectiles/GSTExplodingBarrelActor.h"

UGSTExplodingBarrelAbility::UGSTExplodingBarrelAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGSTExplodingBarrelAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayTagContainer* SourceTags,
                                                    const FGameplayTagContainer* TargetTags,
                                                    FGameplayTagContainer* OptionalRelevantTags) const
{
    return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGSTExplodingBarrelAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{


    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    SpawnedBarrels = 0;
    
    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(ActorInfo->OwnerActor);
    if (!Skimmer || !ExplodingBarrelClass)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    SpawnBarrel();
}

void UGSTExplodingBarrelAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Warning, TEXT("Exploding Barrel Ability failed to commit!"));
    }
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGSTExplodingBarrelAbility::SetupAbilityInput()
{
    // if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    // {
    //     if (SpawnBarrelAction)
    //     {
    //         ASC->BindAbilityActivationToInputComponent(ASC->GetOwnerActor()->InputComponent,
    //             FGameplayAbilityInputBinds(FString("AbilityInput"), FString("AbilityAction"), SpawnBarrelAction));
    //     }
    // }
}

void UGSTExplodingBarrelAbility::SpawnBarrel()
{
    if (SpawnedBarrels >= MaxActiveBarrels)
    {
        UE_LOG(LogTemp, Warning, TEXT("Max barrel limit reached!"));
        return;
    }

    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetCurrentActorInfo()->OwnerActor);
    if (!Skimmer || !ExplodingBarrelClass)
    {
        return;
    }

    FVector SpawnLocation = Skimmer->GetActorLocation() - Skimmer->GetActorForwardVector() * 100.0f;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    AGSTExplodingBarrelActor* Barrel = GetWorld()->SpawnActor<AGSTExplodingBarrelActor>(
        ExplodingBarrelClass, SpawnLocation, SpawnRotation);

    if (Barrel)
    {
        Barrel->InitializeBarrel(Skimmer);
        Barrel->OnBarrelExploded.AddDynamic(this, &UGSTExplodingBarrelAbility::OnBarrelExploded);
        ActiveBarrels.Add(Barrel);
        SpawnedBarrels++;
    }
}

void UGSTExplodingBarrelAbility::OnBarrelExploded(AGSTExplodingBarrelActor* Barrel)
{
    if (ActiveBarrels.Contains(Barrel))
    {
        ActiveBarrels.Remove(Barrel);

        if (ActiveBarrels.Num() == 0)
        {
            EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
        }
    }
}
