#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MainPlayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"


AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = true;

  
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    
    GunStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunStaticMesh"));
    GunStaticMesh->SetupAttachment(SceneRoot);
    GunStaticMesh->SetMobility(EComponentMobility::Movable); 


    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(GunStaticMesh);

    GunType = EGunType::GUN;
}



int32 AGun::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int32 AGun::GetMaxAmmo() const
{
    return MaxAmmo;
}

EGunType AGun::GetGunType() const
{
    return GunType;
}

EItemType AGun::GetAmmoType() const
{
    switch (GunType)
    {
    case EGunType::PISTOL:  return EItemType::PISTOL_BULLET;
    case EGunType::RIFLE:   return EItemType::RIFLE_BULLET;
    case EGunType::SHOTGUN: return EItemType::SHOTGUN_BULLET;
    default:                return EItemType::NONE;
    }
}

int32 AGun::GetAmmoFromInventory(int32 Amount)
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
    if (!Player) return 0;

    EItemType AmmoType = GetAmmoType();
    TMap<EItemType, int32>& Inventory = Player->GetAmmoInventory();

    if (Inventory.Contains(AmmoType) && Inventory[AmmoType] > 0)
    {
        int32 AmmoAvailable = Inventory[AmmoType];
        int32 AmmoToTake = FMath::Min(AmmoAvailable, Amount);
        Inventory[AmmoType] -= AmmoToTake;

        if (Inventory[AmmoType] <= 0)
        {
            Inventory.Remove(AmmoType);
        }

        UE_LOG(LogTemp, Warning, TEXT("인벤토리에서 탄약 사용: %d 남은 탄약: %d"), AmmoToTake, Inventory.Contains(AmmoType) ? Inventory[AmmoType] : 0);

        return AmmoToTake;
    }

    return 0;
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

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
    if (PlayerCharacter)
    {
        AMainPlayerController* PC = Cast<AMainPlayerController>(PlayerCharacter->GetController());
        if (PC)
        {
            PC->UpdateHUD();
        }
    }

}

void AGun::Reload()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("총기의 소유자가 없음!"));
        return;
    }

    int32 AmmoNeeded = MaxAmmo - CurrentAmmo;
    int32 AmmoLoaded = GetAmmoFromInventory(AmmoNeeded);

    if (AmmoLoaded > 0)
    {
        CurrentAmmo += AmmoLoaded;
        UE_LOG(LogTemp, Warning, TEXT("재장전 완료! 현재 탄약: %d / %d"), CurrentAmmo, MaxAmmo);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("재장전할 %s가 부족합니다!"), *UEnum::GetValueAsString(GetAmmoType()));
    }

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
    if (PlayerCharacter)
    {
        AMainPlayerController* PC = Cast<AMainPlayerController>(PlayerCharacter->GetController());
        if (PC)
        {
            PC->UpdateHUD();
        }
    }
}

void AGun::SetCurrentAmmo(int32 NewAmmo)
{
    CurrentAmmo = FMath::Clamp(NewAmmo, 0, MaxAmmo);
}



