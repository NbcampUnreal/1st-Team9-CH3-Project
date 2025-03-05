#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "EnemyCharacter.h"

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;

    // 🔹 충돌 컴포넌트 설정
    collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    collisionComp->SetSphereRadius(13);
    collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    collisionComp->SetCollisionObjectType(ECC_GameTraceChannel1);
    collisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    collisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);  
    collisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    collisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
    RootComponent = collisionComp;

    // 🔹 총알의 메쉬
    bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
    bodyMeshComp->SetupAttachment(collisionComp);
    bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    bodyMeshComp->SetRelativeScale3D(FVector(0.12f));

    // 🔹 총알의 움직임 (발사 속도, 중력 영향)
    movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
    movementComp->SetUpdatedComponent(collisionComp);
    movementComp->InitialSpeed = 5000;
    movementComp->MaxSpeed = 5000;
    movementComp->bShouldBounce = false; 
    movementComp->ProjectileGravityScale = 0.0f;  


    InitialLifeSpan = 3.0f;


    ImpactEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Items/Effects/ParticleSystems/Weapons/AssaultRifle/Impacts/P_AssaultRifle_IH.P_AssaultRifle_IH"));

    if (!ImpactEffect)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ImpactEffect 로드 실패! 경로를 확인하세요."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ ImpactEffect 로드 성공!"));
    }
}


void ABullet::BeginPlay()
{
    Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        // 🔹 적 캐릭터인지 확인
        if (OtherActor->ActorHasTag("Enemy") || OtherActor->IsA(AEnemyCharacter::StaticClass()))
        {
            UE_LOG(LogTemp, Warning, TEXT("💥 총알이 적을 맞춤! (%s)"), *OtherActor->GetName());

            UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetInstigatorController(), this, UDamageType::StaticClass());

            if (ImpactEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, FRotator::ZeroRotator);
            }

            Destroy();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("🧱 총알이 환경 오브젝트에 맞음! (%s)"), *OtherActor->GetName());
        }
    }
}



