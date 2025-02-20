#include "Gun.h"
#include "Kismet/GameplayStatics.h"

AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentAmmo = MaxAmmo;
}

void AGun::Fire()
{
    if (CurrentAmmo > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Firing gun!"));

        if (MuzzleEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, GetActorLocation());
        }

        CurrentAmmo--;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo! Reload needed."));
    }
}

void AGun::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("Reloading gun..."));
    CurrentAmmo = MaxAmmo;
}
