#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"

AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentAmmo = MaxAmmo;
}

<<<<<<< HEAD
=======
int32 AGun::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int32 AGun::GetMaxAmmo() const
{
    return MaxAmmo;
}

>>>>>>> parent of 0478c4b (Merge branch 'dev.player' into Item)
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
<<<<<<< HEAD
                FVector MuzzlePos = GetActorLocation();
                FRotator MuzzleRot = GetActorRotation();
                World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, MuzzleRot);
=======
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
>>>>>>> parent of a5dd2e3 (주석 필요한 것만)
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
