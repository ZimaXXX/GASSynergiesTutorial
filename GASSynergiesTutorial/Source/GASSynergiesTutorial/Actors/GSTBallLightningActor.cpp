#include "GSTBallLightningActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"

AGSTBallLightningActor::AGSTBallLightningActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName("OverlapAll");
	CollisionComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGSTBallLightningActor::OnHit);

	LightningEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningEffect"));
	LightningEffect->SetupAttachment(RootComponent);
}

void AGSTBallLightningActor::InitializeBallLightning(AActor* InOwner, float InLifetime, float InOrbitRadius, float InOrbitSpeed)
{
	OwnerSkimmer = InOwner;
	Lifetime = InLifetime;
	OrbitRadius = InOrbitRadius;
	OrbitSpeed = InOrbitSpeed;
	CollisionComponent->IgnoreActorWhenMoving(OwnerSkimmer, true);

	GetWorldTimerManager().SetTimer(LifetimeTimerHandle, this, &AGSTBallLightningActor::DestroyBallLightning, Lifetime, false);
}

void AGSTBallLightningActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGSTBallLightningActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnerSkimmer)
	{
		float Angle = GetWorld()->TimeSeconds * OrbitSpeed;
		FVector NewLocation = OwnerSkimmer->GetActorLocation() + FVector(FMath::Cos(Angle) * OrbitRadius, FMath::Sin(Angle) * OrbitRadius, 50.0f);
		SetActorLocation(NewLocation);
	}
}

void AGSTBallLightningActor::Destroyed()
{
	OnBallLightningDestroyed.Broadcast(this);
	Super::Destroyed();	
}

void AGSTBallLightningActor::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OwnerSkimmer && OtherActor && OtherActor != OwnerSkimmer)
	{
		OtherActor->Destroy();
	}
}

void AGSTBallLightningActor::DestroyBallLightning()
{
	Destroy();
}
