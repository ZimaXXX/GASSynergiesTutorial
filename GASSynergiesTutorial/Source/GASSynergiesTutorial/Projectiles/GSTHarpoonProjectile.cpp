#include "GSTHarpoonProjectile.h"

#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GASSynergiesTutorial/Actors/GSTPhysicalMaterialWithTags.h"
#include "GASSynergiesTutorial/Core/GSTCharacter.h"
#include "CableComponent.h"
#include "GameplayEffectTypes.h"
#include "GASSynergiesTutorial/Attributes/GSTEquipmentAttributeSet.h"
#include "GASSynergiesTutorial/Enemy/GSTEnemyPawn.h"

AGSTHarpoonProjectile::AGSTHarpoonProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRootComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SceneRootComponent->SetCollisionProfileName("BlockAll");
    SceneRootComponent->bReturnMaterialOnMove = true;
    RootComponent = SceneRootComponent;
    
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);    

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
    CableComponent->SetupAttachment(RootComponent);  // Attach to harpoon
    CableComponent->CableLength = 1500.0f;  // Adjust based on harpoon range
    CableComponent->NumSegments = 10;  // Smoother rope look
    CableComponent->SolverIterations = 8;  // More realistic physics
    CableComponent->SetHiddenInGame(false);  // Ensure visibility
}

void AGSTHarpoonProjectile::InitializeProjectile(AActor* InOwner, float Speed, float InPullVelocityMultiplier)
{   
    OwnerSkimmer = InOwner;
    ProjectileMovement->InitialSpeed = Speed;
    ProjectileMovement->MaxSpeed = Speed;
    PullVelocityMultiplier = InPullVelocityMultiplier;
    float AttributeRange = BaseRange;
    if (OwnerAttributes && OwnerASC)
    {
        AttributeRange = OwnerAttributes->GetHarpoonRange();
    }
    CableComponent->CableLength = AttributeRange;

    SetLifeSpan(AttributeRange / Speed); // Destroy after reaching range

    if (OwnerSkimmer)
    {
        CableComponent->SetAttachEndTo(OwnerSkimmer, FName("RootComponent"));
        // Ignore the skimmer to prevent self-collision
        MeshComponent->IgnoreActorWhenMoving(OwnerSkimmer, true);

        // Ignore collisions dynamically
        UPrimitiveComponent* SkimmerRoot = Cast<UPrimitiveComponent>(OwnerSkimmer->GetRootComponent());
        if (SkimmerRoot)
        {
            SkimmerRoot->IgnoreActorWhenMoving(this, true);
        }
    }
}

void AGSTHarpoonProjectile::BeginPlay()
{
    Super::BeginPlay();

    AActor* OwnerActor = GetOwner();
    if (OwnerActor)
    {
        OwnerASC = OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
        OwnerAttributes = Cast<UGSTEquipmentAttributeSet>(OwnerASC->GetAttributeSet(UGSTEquipmentAttributeSet::StaticClass()));
    }
}

bool AGSTHarpoonProjectile::IsValidHarpoonSurface(const FHitResult& Hit) const
{
    int32 AttributeHarpoonHardnessLevel = BaseHardnessLevel;
    if (OwnerAttributes && OwnerASC)
    {
        AttributeHarpoonHardnessLevel = OwnerAttributes->GetHarpoonHardness();
    }
    FGameplayTagContainer AllowedTags = ReceiveGetAllowedMaterialTags(AttributeHarpoonHardnessLevel);
    if (Hit.PhysMaterial.IsValid())
    {
        UGSTPhysicalMaterialWithTags* PhysMat = Cast<UGSTPhysicalMaterialWithTags>(Hit.PhysMaterial.Get());
        if (PhysMat && AllowedTags.HasAll(PhysMat->MaterialTags))
        {
            return true;
        }
    }

    return false;
}

void AGSTHarpoonProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, bool bSelfMoved,
                                      FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                                      const FHitResult& Hit)
{
    if (!IsValidHarpoonSurface(Hit))
    {
        if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
        {
            OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
        }

        AGSTEnemyPawn* Enemy = Cast<AGSTEnemyPawn>(OtherActor);
        if (Enemy)
        {
            ApplyDamageToEnemy(Enemy);
        }
        
        UE_LOG(LogTemp, Warning, TEXT("Harpoon hit an invalid surface! Destroying."));
        Destroy();
        return;
    }
    
    AGSTCharacter* Skimmer = Cast<AGSTCharacter>(OwnerSkimmer);
    if (Skimmer)
    {
        FVector PullDirection = (HitLocation - Skimmer->GetActorLocation()).GetSafeNormal();

        if (UFloatingPawnMovement* FloatingMovement = Skimmer->FindComponentByClass<UFloatingPawnMovement>())
        {
            PullVelocityMultiplier = 5;
            FloatingMovement->Velocity = PullDirection * FloatingMovement->MaxSpeed * PullVelocityMultiplier;
        }
    }

    Destroy();
}

void AGSTHarpoonProjectile::ApplyDamageToEnemy(AActor* HitEnemy)
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
        AttributeDamage = OwnerAttributes->GetHarpoonDamage();
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

        UE_LOG(LogTemp, Warning, TEXT("Harpoon hit enemy: %s | Applied Damage: %f"), *EnemyPawn->GetName(), AttributeDamage);
    }
}