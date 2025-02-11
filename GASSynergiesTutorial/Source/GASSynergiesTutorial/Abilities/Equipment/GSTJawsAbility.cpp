#include "GSTJawsAbility.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"
#include "GASSynergiesTutorial/Core/GSTCharacter.h"

class UGSTEquipmentAttributeSet;

UGSTJawsAbility::UGSTJawsAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Tool.Jaws"));
}

bool UGSTJawsAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
    const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    // Check if the owner is a valid skimmer character
    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(ActorInfo->OwnerActor);
    if (!Skimmer)
    {
        UE_LOG(LogTemp, Warning, TEXT("Jaws Ability Failed: Owner is not a Skimmer!"));
        return false;
    }

    // Check if BurrowEffect is set
    if (!BurrowEffect)
    {
        UE_LOG(LogTemp, Warning, TEXT("Jaws Ability Failed: BurrowEffect is missing!"));
        return false;
    }

    // Check if the skimmer is over a valid burrow material
    if (!Skimmer->IsSkimmerOverMaterial(FGameplayTag::RequestGameplayTag("Material.Sand")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Jaws Ability Failed: Skimmer is not over a burrowable surface!"));
        return false;
    }

    return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGSTJawsAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
                                      const FGameplayAbilityActorInfo* ActorInfo, 
                                      const FGameplayAbilityActivationInfo ActivationInfo, 
                                      const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


    
    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        if (AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetOwningActorFromActorInfo()->GetInstigatorController()->GetPawn()); BurrowEffect)
        {
            
            // Get the max burrow time from attributes
            float BurrowDuration = OwnerAttributes ? OwnerAttributes->BurrowMaxTime.GetCurrentValue() : 5.0f; // Default: 5 seconds

            // Apply burrow effect with dynamic duration
            FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(BurrowEffect, 1.0f, ASC->MakeEffectContext());
            EffectSpec.Data->SetDuration(BurrowDuration, true);
            ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
            //Start Burrowing On Skimmer
            Skimmer->StartBurrowing();
            // Set a timer to auto-end burrow
            GetWorld()->GetTimerManager().SetTimer(BurrowTimerHandle, this, &UGSTJawsAbility::AutoEndBurrow, BurrowDuration, false);
            UE_LOG(LogTemp, Warning, TEXT("Jaws Ability Activated: Skimmer is Buried & Invulnerable for %.2f seconds!"), BurrowDuration);
        }
    }

    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        FGameplayTagContainer ActiveTags;
        ASC->GetOwnedGameplayTags(ActiveTags);

        UE_LOG(LogTemp, Warning, TEXT("Active Gameplay Tags: %s"), *ActiveTags.ToString());
    }
}

void UGSTJawsAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, 
                                 const FGameplayAbilityActorInfo* ActorInfo, 
                                 const FGameplayAbilityActivationInfo ActivationInfo, 
                                 bool bReplicateEndAbility, bool bWasCancelled)
{

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))//commit on endability so cooldown starts to count from burrowing up
    {
        UE_LOG(LogTemp, Warning, TEXT("JawsAbility failed to commit!"));
    }
    
    if(GetOwningActorFromActorInfo() && GetOwningActorFromActorInfo()->GetInstigatorController())//may fail on closing PIE
    {
        AGSTCharacter* Skimmer = Cast<AGSTCharacter>(GetOwningActorFromActorInfo()->GetInstigatorController()->GetPawn());
        if (Skimmer)
        {
            Skimmer->EndBurrowing();
        }
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

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGSTJawsAbility::AutoEndBurrow()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


