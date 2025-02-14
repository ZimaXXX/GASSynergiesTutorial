// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "InputMappingContext.h"
#include "GSTCharacter.generated.h"

class UGSTEquipmentManagerComponent;
class UGSTEquipmentDataAsset;
class UFloatingPawnMovement;
class UGameplayAbility;
class UGSTEquipmentAttributeSet;
class UGSTAbilitySystemComponent;

UCLASS(Blueprintable)
class AGSTCharacter : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	AGSTCharacter();

	virtual void BeginPlay() override;
	
	bool IsSkimmerOverMaterial(FGameplayTag MaterialTag) const;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	void Move(const FInputActionValue& Value);
	
	
	void OnBurrowStarted();
	
	/** Implement GAS interface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	/** Ability System Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UGSTAbilitySystemComponent* AbilitySystemComponent;

	/** Equipment Manager Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UGSTEquipmentManagerComponent* EquipmentManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* FloatingMovement;

	/** Attribute Set for character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	UGSTEquipmentAttributeSet* AttributeSet;
	
	/** DataTable containing attribute values */
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	UDataTable* EquipmentAttributesDataTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	UGSTEquipmentDataAsset* EquipmentDataAsset;

	/** Grants default abilities defined in EquipmentDataAsset */
	void GrantDefaultAbilities();
public:
	/** Moves the skimmer underground */
	void StartBurrowing();

	/** Moves the skimmer back to the surface */
	void EndBurrowing();
	void OnBurrowFinished();

	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool IsBuried() const;

private:
	float OriginalZ;
	float BurrowedZ;
	FTimerHandle MovementTimerHandle;

	void MoveToBurrow();
	void MoveToSurface();

public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

