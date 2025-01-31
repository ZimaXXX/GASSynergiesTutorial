#include "GSTEquipmentAbility.h"
#include <GASSynergiesTutorial/Actors/GSTPhysicalMaterialWithTags.h>
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GASSynergiesTutorial/Actors/GSTPhysicalMaterialWithTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UGSTEquipmentAbility::UGSTEquipmentAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGSTEquipmentAbility::IsSkimmerOverMaterial(FGameplayTag MaterialTag)
{
	if (AActor* OwnerActor = GetOwningActorFromActorInfo())
	{
		FVector StartLocation = OwnerActor->GetActorLocation();
		FVector EndLocation = StartLocation + FVector(0, 0, -500); // Raycast 500 units downward

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwnerActor);

		// Perform a LineTrace downward to check surface below the skimmer
		if (OwnerActor->GetWorld()->LineTraceSingleByChannel(
			HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
		{
			UGSTPhysicalMaterialWithTags* PhysMat = Cast<UGSTPhysicalMaterialWithTags>(HitResult.PhysMaterial.Get());
			if (PhysMat && PhysMat->MaterialTags.HasTag(MaterialTag))
			{
				return true;
			}
		}
	}
	return false;
}