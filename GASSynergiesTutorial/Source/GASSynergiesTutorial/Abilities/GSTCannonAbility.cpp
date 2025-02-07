#include "GSTCannonAbility.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"
#include "GASSynergiesTutorial/Core/GSTCharacter.h"
#include "GASSynergiesTutorial/Projectiles/GSTCannonProjectile.h"

UGSTCannonAbility::UGSTCannonAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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

    // Start continuous enemy lookup
    LookForEnemies();
}

void UGSTCannonAbility::LookForEnemies()
{
    //DrawDebugAimAssist();
    // Look for closest enemy
    AActor* ClosestEnemy = FindClosestValidEnemy();

    if (ClosestEnemy)
    {
        // Check if we can fire at this enemy
        CheckIfCanFire();
    }

    // Set a repeating timer to continuously look for enemies
    GetWorld()->GetTimerManager().SetTimer(EnemyLookupTimerHandle, this, &UGSTCannonAbility::LookForEnemies, LookUpRate, false);
}

void UGSTCannonAbility::CheckIfCanFire()
{
    if (IsInCooldown())
    {
        return; // Don't fire if still on cooldown
    }

    AActor* ClosestEnemy = FindClosestValidEnemy();
    if (ClosestEnemy)
    {
        FireCannon(ClosestEnemy);
    }
}

void UGSTCannonAbility::FireCannon(AActor* TargetActor)
{
    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetCurrentActorInfo()->OwnerActor);
    if (!Skimmer || !CannonProjectileClass)
    {
        return;
    }

    FVector SpawnLocation = Skimmer->GetActorLocation();
    FVector RightDirection = Skimmer->GetActorRightVector();
    FVector LeftDirection = -RightDirection;
    FRotator SpawnRotation;

    if (TargetActor)
    {
        FVector TargetDirection = (TargetActor->GetActorLocation() - SpawnLocation).GetSafeNormal();
        SpawnRotation = TargetDirection.Rotation();
    }
    else
    {
        bool bFireRight = FMath::RandBool();
        FVector ChosenDirection = bFireRight ? RightDirection : LeftDirection;
        SpawnRotation = ChosenDirection.Rotation();
    }

    AGSTCannonProjectile* Projectile = GetWorld()->SpawnActor<AGSTCannonProjectile>(
        CannonProjectileClass, SpawnLocation, SpawnRotation);

    if (Projectile)
    {
        Projectile->InitializeProjectile(Skimmer, MaxRange);
    }

    // Start cooldown timer
    UAbilitySystemComponent* ASC = Skimmer->GetAbilitySystemComponent();
    if (ASC)
    {
        const UGSTEquipmentAttributeSet* Attributes = ASC->GetSet<UGSTEquipmentAttributeSet>();
        if (Attributes)
        {
            float FireRate = Attributes->GetFireRate();
            float FireCooldown = (FireRate > 0.0f) ? (1.0f / FireRate) : 1.0f;
            GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UGSTCannonAbility::CheckIfCanFire, FireCooldown, false);
        }
    }
}

AActor* UGSTCannonAbility::FindClosestValidEnemy()
{
    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetCurrentActorInfo()->OwnerActor);
    if (!Skimmer)
    {
        return nullptr;
    }

    FVector StartLocation = Skimmer->GetActorLocation();
    FVector RightDirection = Skimmer->GetActorRightVector();
    FVector LeftDirection = -RightDirection;

    float ClosestDistance = MaxRange;
    AActor* ClosestEnemy = nullptr;

    TArray<AActor*> PotentialTargets;
    UKismetSystemLibrary::SphereOverlapActors(this, StartLocation, MaxRange,
        { UEngineTypes::ConvertToObjectType(ECC_Pawn) }, nullptr, { Skimmer }, PotentialTargets);

    for (AActor* Target : PotentialTargets)
    {
        if (!Target || Target == Skimmer)
        {
            continue;
        }

        FVector DirectionToTarget = (Target->GetActorLocation() - StartLocation).GetSafeNormal();
        float RightDotProduct = FVector::DotProduct(DirectionToTarget, RightDirection);
        float TargetAngle = FMath::Acos(RightDotProduct) * (180.0f / PI);

        // Ensure the target is within the allowed angle range for BOTH sides
        if (TargetAngle <= AllowedAimAngle * 0.5f || TargetAngle >= (180.0f - AllowedAimAngle * 0.5f))
        {
            float Distance = FVector::Dist(Target->GetActorLocation(), StartLocation);
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestEnemy = Target;
            }
        }
    }

    return ClosestEnemy;
}

bool UGSTCannonAbility::IsInCooldown()
{
    return GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle);
}

void UGSTCannonAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                   bool bReplicateEndAbility,
                                   bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    
    // Clear both timers when ability ends
    GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(EnemyLookupTimerHandle);
}

void UGSTCannonAbility::DrawDebugAimAssist()
{

    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetCurrentActorInfo()->OwnerActor);
    if (!Skimmer)
    {
        return;
    }

    FVector NewStartLocation = Skimmer->GetActorLocation();
    FVector RightDirection = Skimmer->GetActorRightVector();
    FVector LeftDirection = -RightDirection;
    
    FVector RightUpLimitDirection = RightDirection.RotateAngleAxis(-AllowedAimAngle * 0.5f, FVector::UpVector);
    FVector RightDownLimitDirection = RightDirection.RotateAngleAxis(AllowedAimAngle * 0.5f, FVector::UpVector);

    DrawDebugLine(GetWorld(), NewStartLocation, NewStartLocation + RightUpLimitDirection * MaxRange, FColor::Green, false, 0.5f, 0, 2.0f);
    DrawDebugLine(GetWorld(), NewStartLocation, NewStartLocation + RightDownLimitDirection * MaxRange, FColor::Green, false, 0.5f, 0, 2.0f);

    FVector LeftUpLimitDirection = LeftDirection.RotateAngleAxis(-AllowedAimAngle * 0.5f, FVector::UpVector);
    FVector LeftDownLimitDirection = LeftDirection.RotateAngleAxis(AllowedAimAngle * 0.5f, FVector::UpVector);

    DrawDebugLine(GetWorld(), NewStartLocation, NewStartLocation + LeftUpLimitDirection * MaxRange, FColor::Green, false, 0.5f, 0, 2.0f);
    DrawDebugLine(GetWorld(), NewStartLocation, NewStartLocation + LeftDownLimitDirection * MaxRange, FColor::Green, false, 0.5f, 0, 2.0f);
}