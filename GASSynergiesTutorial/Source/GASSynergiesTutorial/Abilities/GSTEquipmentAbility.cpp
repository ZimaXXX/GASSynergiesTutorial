#include "GSTEquipmentAbility.h"
#include <GASSynergiesTutorial/Actors/GSTPhysicalMaterialWithTags.h>
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"

UGSTEquipmentAbility::UGSTEquipmentAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGSTEquipmentAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* OwnerActor = ActorInfo->OwnerActor.Get();
	if (OwnerActor)
	{
		OwnerASC = OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
		OwnerAttributes = Cast<UGSTEquipmentAttributeSet>(OwnerASC->GetAttributeSet(UGSTEquipmentAttributeSet::StaticClass()));
	}
}
