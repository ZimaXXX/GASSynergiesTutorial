#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSTBallLightningActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallLightningDestroyed, AGSTBallLightningActor*, BallLightning);

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class GASSYNERGIESTUTORIAL_API AGSTBallLightningActor : public AActor
{
	GENERATED_BODY()

public:    
	AGSTBallLightningActor();

	void InitializeBallLightning(AActor* InOwner, float InLifetime, float InOrbitRadius, float InOrbitSpeed, float InAngleOffset, TArray<
	                             AGSTBallLightningActor*>
	                             ActiveLightnings);
	
	FOnBallLightningDestroyed OnBallLightningDestroyed;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			   bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* LightningEffect;

	UPROPERTY()
	AActor* OwnerSkimmer;

	float OrbitRadius = 200.0f;
	float OrbitSpeed = 100.0f;
	float Lifetime = 0.f;
	float AngleOffset = 0.f;

	FTimerHandle LifetimeTimerHandle;

	void DestroyBallLightning();
};
