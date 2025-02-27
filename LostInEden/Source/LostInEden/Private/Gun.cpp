#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = true;

    GunStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunStaticMesh"));
    SetRootComponent(GunStaticMesh);

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(GunStaticMesh);

    BulletFactory = nullptr;  // 🚨 안전성을 위해 초기화

    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
}

int32 AGun::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int32 AGun::GetMaxAmmo() const
{
    return MaxAmmo;
}

void AGun::Fire()
{
    if (CurrentAmmo > 0)
    {
        CurrentAmmo--;

        if (!BulletFactory)
        {
            UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음! BP_Bullet을 자동 할당합니다."));
            AutoAssignBulletFactory();  // Bullet Factory 자동 할당
        }

        if (BulletFactory)
        {
            UWorld* World = GetWorld();
            if (World)
            {
                FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
                FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
                FVector ShotDirection = MuzzleRot.Vector();

                ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
                if (SpawnedBullet)
                {
                    UProjectileMovementComponent* BulletMovement = SpawnedBullet->FindComponentByClass<UProjectileMovementComponent>();
                    if (BulletMovement)
                    {
                        BulletMovement->Velocity = ShotDirection * BulletMovement->InitialSpeed;
                    }

                    UE_LOG(LogTemp, Warning, TEXT("총알 스폰 성공!"));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Bullet Factory 자동 할당 실패! 블루프린트 설정을 확인하세요."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
    }
}

void AGun::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("AGun 기본 Reload() 실행됨"));
    CurrentAmmo = MaxAmmo;
}

void AGun::BeginPlay()
{
    Super::BeginPlay();

    // BulletFactory 자동 설정
    AutoAssignBulletFactory();
}

void AGun::AutoAssignBulletFactory()
{
    if (BulletFactory)
    {
        return; // 이미 설정되어 있으면 변경하지 않음
    }

    static ConstructorHelpers::FClassFinder<ABullet> BulletBP(TEXT("/Game/Items/Blueprints/BP_Bullet.BP_Bullet'"));

    if (BulletBP.Succeeded())
    {
        BulletFactory = BulletBP.Class;
        UE_LOG(LogTemp, Warning, TEXT("Bullet Factory 자동 설정 완료! BP_Bullet이 설정되었습니다."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet Factory 자동 설정 실패! 블루프린트 경로를 확인하세요."));
        BulletFactory = nullptr; // 실패 시 명확하게 `nullptr`로 설정
    }
}

