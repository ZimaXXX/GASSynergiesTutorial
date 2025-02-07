#include "GSTCannonTargetActor.h"

#include "Abilities/GameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AGSTCannonTargetActor::AGSTCannonTargetActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGSTCannonTargetActor::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    // Start scanning for enemies every 0.1s
    GetWorld()->GetTimerManager().SetTimer(TargetScanTimer, this, &AGSTCannonTargetActor::ScanForTarget, 0.1f, true, 0.0f);
}

void AGSTCannonTargetActor::ConfirmTargetingAndContinue()
{
    AActor* ClosestEnemy = FindClosestValidEnemy();

    if (ClosestEnemy)
    {
        FHitResult HitResult;
        HitResult.HitObjectHandle = FActorInstanceHandle(ClosestEnemy);
        HitResult.Location = ClosestEnemy->GetActorLocation();
        HitResult.ImpactPoint = ClosestEnemy->GetActorLocation();
        HitResult.bBlockingHit = true;

        FGameplayAbilityTargetDataHandle TargetData;
        FGameplayAbilityTargetData_SingleTargetHit* TargetHit = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
        TargetData.Add(TargetHit);

        TargetDataReadyDelegate.Broadcast(TargetData);
    }
    else
    {
        // No target found, return empty target data
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
    }
}


/** Scans for the closest enemy and triggers firing when a valid target is found */
void AGSTCannonTargetActor::ScanForTarget()
{
    // ✅ Always visualize valid shooting angles
    DrawDebugAimAssist();

    AActor* ClosestEnemy = FindClosestValidEnemy();

    if (ClosestEnemy)
    {
        FHitResult HitResult;
        HitResult.HitObjectHandle = FActorInstanceHandle(ClosestEnemy);
        HitResult.Location = ClosestEnemy->GetActorLocation();
        HitResult.ImpactPoint = ClosestEnemy->GetActorLocation();
        HitResult.bBlockingHit = true;

        FGameplayAbilityTargetDataHandle TargetData;
        FGameplayAbilityTargetData_SingleTargetHit* TargetHit = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
        TargetData.Add(TargetHit);

        TargetDataReadyDelegate.Broadcast(TargetData);
    }
}



/** Finds the closest enemy within range */
AActor* AGSTCannonTargetActor::FindClosestValidEnemy()
{
    if (!OwningAbility)
    {
        return nullptr;
    }

    APawn* Skimmer = Cast<APawn>(OwningAbility->GetOwningActorFromActorInfo());
    if (!Skimmer)
    {
        return nullptr;
    }

    FVector NewStartLocation = Skimmer->GetActorLocation();
    FVector RightDirection = Skimmer->GetActorRightVector();

    float ClosestDistance = MaxTargetingRange;
    AActor* ClosestEnemy = nullptr;

    // ✅ Find all potential targets in range
    TArray<AActor*> PotentialTargets;
    UKismetSystemLibrary::SphereOverlapActors(this, NewStartLocation, MaxTargetingRange,
        { UEngineTypes::ConvertToObjectType(ECC_Pawn) }, nullptr, { Skimmer }, PotentialTargets);

    for (AActor* Target : PotentialTargets)
    {
        if (!Target || Target == Skimmer)
        {
            continue;
        }

        FVector DirectionToTarget = (Target->GetActorLocation() - NewStartLocation).GetSafeNormal();
        float RightDotProduct = FVector::DotProduct(DirectionToTarget, RightDirection);
        float TargetAngle = FMath::Acos(RightDotProduct) * (180.0f / PI);

        // ✅ Ensure the target is within the allowed angle range for BOTH sides
        if (TargetAngle <= AllowedAimAngle * 0.5f || TargetAngle >= (180.0f - AllowedAimAngle * 0.5f))
        {
            float Distance = FVector::Dist(Target->GetActorLocation(), NewStartLocation);
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestEnemy = Target;
            }
        }
    }

    return ClosestEnemy;
}





/** Stops scanning for targets */
void AGSTCannonTargetActor::StopScanning()
{
    GetWorld()->GetTimerManager().ClearTimer(TargetScanTimer);
}



void AGSTCannonTargetActor::DrawDebugAimAssist()
{
    if (!OwningAbility)
    {
        return;
    }

    APawn* Skimmer = Cast<APawn>(OwningAbility->GetOwningActorFromActorInfo());
    if (!Skimmer)
    {
        return;
    }

    FVector NewStartLocation = Skimmer->GetActorLocation();
    FVector RightDirection = Skimmer->GetActorRightVector();
    FVector LeftDirection = -RightDirection;
    
    FVector RightUpLimitDirection = RightDirection.RotateAngleAxis(-AllowedAimAngle * 0.5f, FVector::UpVector);
    FVector RightDownLimitDirection = RightDirection.RotateAngleAxis(AllowedAimAngle * 0.5f, FVector::UpVector);

    DrawDebugLine(GetWorld(), NewStartLocation, NewStartLocation + RightUpLimitDirection * MaxTargetingRange, FColor::Green, false, 0.5f, 0, 2.0f);
    DrawDebugLine(GetWorld(), NewStartLocation, NewStartLocation + RightDownLimitDirection * MaxTargetingRange, FColor::Green, false, 0.5f, 0, 2.0f);

    FVector LeftUpLimitDirection = LeftDirection.RotateAngleAxis(-AllowedAimAngle * 0.5f, FVector::UpVector);
    FVector LeftDownLimitDirection = LeftDirection.RotateAngleAxis(AllowedAimAngle * 0.5f, FVector::UpVector);

    DrawDebugLine(GetWorld(), NewStartLocation, NewStartLocation + LeftUpLimitDirection * MaxTargetingRange, FColor::Green, false, 0.5f, 0, 2.0f);
    DrawDebugLine(GetWorld(), NewStartLocation, NewStartLocation + LeftDownLimitDirection * MaxTargetingRange, FColor::Green, false, 0.5f, 0, 2.0f);
}


