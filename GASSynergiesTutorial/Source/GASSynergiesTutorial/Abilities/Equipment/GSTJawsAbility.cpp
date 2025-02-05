#include "GSTJawsAbility.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"
#include "GASSynergiesTutorial/Core/GSTCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

class UGSTEquipmentAttributeSet;

UGSTJawsAbility::UGSTJawsAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Tool.Jaws"));
}

void UGSTJawsAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
                                      const FGameplayAbilityActorInfo* ActorInfo, 
                                      const FGameplayAbilityActivationInfo ActivationInfo, 
                                      const FGameplayEventData* TriggerEventData)
{
    if (!IsSkimmerOverMaterial(FGameplayTag::RequestGameplayTag("Material.Sand")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Jaws Ability Failed: Not Over Sand!"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        if (BurrowEffect)
        {
            AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetOwningActorFromActorInfo()->GetInstigatorController()->GetPawn());
            if (Skimmer)
            {
                Skimmer->StartBurrowing(); // ✅ Tell character to burrow
            }
            
            // ✅ Get the max burrow time from attributes
            const UGSTEquipmentAttributeSet* Attributes = Cast<UGSTEquipmentAttributeSet>(ASC->GetAttributeSet(UGSTEquipmentAttributeSet::StaticClass()));
            float BurrowDuration = Attributes ? Attributes->BurrowMaxTime.GetCurrentValue() : 5.0f; // Default: 5 seconds

            // ✅ Apply burrow effect with dynamic duration
            FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(BurrowEffect, 1.0f, ASC->MakeEffectContext());
            EffectSpec.Data->SetDuration(BurrowDuration, true);
            ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

            // ✅ Set a timer to auto-end burrow
            GetWorld()->GetTimerManager().SetTimer(BurrowTimerHandle, this, &UGSTJawsAbility::AutoEndBurrow, BurrowDuration, false);

            UE_LOG(LogTemp, Warning, TEXT("Jaws Ability Activated: Skimmer is Buried & Invulnerable for %.2f seconds!"), BurrowDuration);
        }
    }    
}

void UGSTJawsAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, 
                                 const FGameplayAbilityActorInfo* ActorInfo, 
                                 const FGameplayAbilityActivationInfo ActivationInfo, 
                                 bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    
    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetOwningActorFromActorInfo()->GetInstigatorController()->GetPawn());
    if (Skimmer)
    {
        Skimmer->EndBurrowing(); // ✅ Tell character to resurface
    }

    if(BurrowTimerHandle.IsValid())
    {
        BurrowTimerHandle.Invalidate();
    }
    
    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        if (ActiveEffectHandle.IsValid())
        {
            ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Jaws Ability Ended: Skimmer is Out of Burrow!"));
}

void UGSTJawsAbility::AutoEndBurrow()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


