#include "GSTCannonAbility.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "GASSynergiesTutorial/Core/GSTCharacter.h"
#include "GASSynergiesTutorial/Projectiles/GSTCannonProjectile.h"
#include "TargetActors/GSTCannonTargetActor.h"

UGSTCannonAbility::UGSTCannonAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGSTCannonAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayTagContainer* SourceTags,
                                           const FGameplayTagContainer* TargetTags,
                                           FGameplayTagContainer* OptionalRelevantTags) const
{
    return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGSTCannonAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // ✅ Create the targeting actor
    AGSTCannonTargetActor* TargetActor = GetWorld()->SpawnActor<AGSTCannonTargetActor>(TargetActorClass);
    if (!TargetActor)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }
    
    TargetActor->StartTargeting(this);

    TargetActor->TargetDataReadyDelegate.AddUObject(this, &UGSTCannonAbility::OnTargetDataReceived);
}


void UGSTCannonAbility::OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& Data)
{
    if (Data.Data.IsValidIndex(0))
    {
        const FGameplayAbilityTargetData_SingleTargetHit* TargetData =
            static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(Data.Data[0].Get());

        if (TargetData && IsValid(TargetData->HitResult.GetActor()))
        {
            FireCannon(TargetData->HitResult.GetActor());
            return;
        }
    }

    // No valid target found, just fire forward
    FireCannon(nullptr);
}

void UGSTCannonAbility::FireCannon(AActor* TargetActor)
{
    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetCurrentActorInfo()->OwnerActor);
    if (!Skimmer || !CannonProjectileClass)
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
        return;
    }

    FVector SpawnLocation = Skimmer->GetActorLocation(); // ✅ Spawn inside the ship
    FRotator SpawnRotation;

    if (TargetActor)
    {
        FVector TargetDirection = (TargetActor->GetActorLocation() - SpawnLocation).GetSafeNormal();
        SpawnRotation = TargetDirection.Rotation();
    }
    else
    {
        FVector RightDirection = Skimmer->GetActorRightVector();
        SpawnRotation = RightDirection.Rotation();
    }

    // ✅ Spawn projectile inside the ship
    AGSTCannonProjectile* Projectile = GetWorld()->SpawnActor<AGSTCannonProjectile>(
        CannonProjectileClass, SpawnLocation, SpawnRotation);

    if (Projectile)
    {
        Projectile->InitializeProjectile(Skimmer, MaxRange);
    }
}


void UGSTCannonAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                   bool bReplicateEndAbility,
                                   bool bWasCancelled)
{
    GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
