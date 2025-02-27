#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = true;
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

                FVector MuzzlePos;
                FRotator MuzzleRot;
                FVector ShotDirection;

               
                if (MuzzleLocation)
                {
                    MuzzlePos = MuzzleLocation->GetComponentLocation();
                    MuzzleRot = MuzzleLocation->GetComponentRotation();
                }
                else
                {
                    MuzzlePos = GetActorLocation();
                    MuzzleRot = GetActorRotation();
                }

                ShotDirection = MuzzleRot.Vector();

           
                World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, MuzzleRot);


               
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
