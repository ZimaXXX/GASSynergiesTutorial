#include "GSTEnemyPawn.h"
#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "GASSynergiesTutorial/Attributes/GSTEnemyAttributeSet.h"

AGSTEnemyPawn::AGSTEnemyPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    EnemyAttributes = CreateDefaultSubobject<UGSTEnemyAttributeSet>(TEXT("EnemyAttributes"));
}

UAbilitySystemComponent* AGSTEnemyPawn::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AGSTEnemyPawn::BeginPlay()
{
    Super::BeginPlay();

    if (AbilitySystemComponent && EnemyAttributes)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
        
        // Bind the health change event
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            EnemyAttributes->GetHealthAttribute()).AddUObject(this, &AGSTEnemyPawn::HandleHealthChanged);
    }
}

void AGSTEnemyPawn::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
    float NewHealth = Data.NewValue;
    float MaxHealthValue = GetMaxHealth();

    // Broadcast the health change to listeners (such as UI)
    OnHealthChangedDelegate.Broadcast(NewHealth, MaxHealthValue);
}

float AGSTEnemyPawn::GetHealth() const
{
    return EnemyAttributes->GetHealth();
}

float AGSTEnemyPawn::GetMaxHealth() const
{
    return EnemyAttributes->GetMaxHealth();
}
