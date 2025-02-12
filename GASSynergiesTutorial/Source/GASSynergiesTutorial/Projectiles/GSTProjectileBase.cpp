// Fill out your copyright notice in the Description page of Project Settings.


#include "GSTProjectileBase.h"

#include "GameplayEffect.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"
#include "GASSynergiesTutorial/Enemy/GSTEnemyPawn.h"


// Sets default values
AGSTProjectileBase::AGSTProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGSTProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		OwnerASC = OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
		OwnerAttributes = Cast<UGSTEquipmentAttributeSet>(OwnerASC->GetAttributeSet(UGSTEquipmentAttributeSet::StaticClass()));
	}
}

// Called every frame
void AGSTProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AGSTProjectileBase::ApplyDamageToEnemy(AActor* HitEnemy)
{
	if (!DamageEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Harpoon Projectile: No DamageEffectClass assigned!"));
		return;
	}

	AGSTEnemyPawn* EnemyPawn = Cast<AGSTEnemyPawn>(HitEnemy);
	if (!EnemyPawn)
	{
		return;
	}

	UAbilitySystemComponent* EnemyASC = EnemyPawn->GetAbilitySystemComponent();
	if (!EnemyASC)
	{
		return;
	}

	// Retrieve the Harpoon Damage attribute value
	float AttributeDamage = BaseDamage;
	if (OwnerAttributes && OwnerASC)
	{
		AttributeDamage = GetDamageFromAttribute();
	}

	if(AttributeDamage > 0)
	{
		AttributeDamage *= -1.f; // Damage needs to be negative
	}
   
	// Apply the GameplayEffect with the dynamic damage value
	FGameplayEffectSpecHandle DamageEffect = EnemyASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EnemyASC->MakeEffectContext());
	if (DamageEffect.IsValid())
	{
		DamageEffect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Damage")), AttributeDamage);
		EnemyASC->ApplyGameplayEffectSpecToSelf(*DamageEffect.Data.Get());

		UE_LOG(LogTemp, Warning, TEXT("Hit enemy: %s | Applied Damage: %f"), *EnemyPawn->GetName(), AttributeDamage);
	}
}
