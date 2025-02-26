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

        if (BulletFactory)
        {
            UWorld* World = GetWorld();
            if (World)
            {
                FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
                FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
                FVector ShotDirection = MuzzleRot.Vector();

                // 🔹 총알 생성
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

                // 🔹 라인트레이스를 제거하거나, 총알이 맞았을 때만 트리거
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
    }
}



void AGun::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("재장전!"));
    CurrentAmmo = MaxAmmo;
}

void AGun::BeginPlay()
{
    Super::BeginPlay();

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Gun: 플레이어 캐릭터를 찾을 수 없습니다!"));
        return;
    }

    FName WeaponSocket = "GunSocket_R";
    AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);

    UE_LOG(LogTemp, Warning, TEXT("%s가 플레이어 손에 장착됨!"), *GetName());
}

