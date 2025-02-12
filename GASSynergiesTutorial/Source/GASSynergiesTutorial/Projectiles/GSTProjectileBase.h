// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSTProjectileBase.generated.h"

class UGSTEquipmentAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS(Abstract)
class GASSYNERGIESTUTORIAL_API AGSTProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGSTProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Damage amount applied when harpoon hits an enemy */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float BaseDamage = 10.0f;
	
	/** The GameplayEffect to apply for damage */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY()
	UAbilitySystemComponent* OwnerASC;

	UPROPERTY()
	const UGSTEquipmentAttributeSet* OwnerAttributes;

	void ApplyDamageToEnemy(AActor* HitEnemy);

	virtual float GetDamageFromAttribute() PURE_VIRTUAL(AGSTProjectileBase::GetDamageFromAttribute, return 0.f; );
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
