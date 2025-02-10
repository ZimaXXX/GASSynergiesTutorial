#include "GSTEnemyAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGSTEnemyAttributeSet::UGSTEnemyAttributeSet()
{
	Health.SetBaseValue(100.0f);
	Health.SetCurrentValue(100.0f);
	MaxHealth.SetBaseValue(100.0f);
	MaxHealth.SetCurrentValue(100.0f);
}

void UGSTEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));

		if (Health.GetCurrentValue() <= 0.0f)
		{
			AActor* Owner = GetOwningActor();
			if (Owner)
			{
				Owner->Destroy(); // Enemy dies when health reaches 0
			}
		}
	}
}