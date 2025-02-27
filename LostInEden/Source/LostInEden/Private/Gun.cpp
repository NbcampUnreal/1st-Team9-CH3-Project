#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"

AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentAmmo = MaxAmmo;
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
                FVector MuzzlePos = GetActorLocation();
                FRotator MuzzleRot = GetActorRotation();
                World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, MuzzleRot);
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
