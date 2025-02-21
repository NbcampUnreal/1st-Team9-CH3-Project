#include "Shotgun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"

AShotgun::AShotgun()
{
    Damage = 10.0f;
    FireRate = 1.0f;
    MaxAmmo = 8;
    CurrentAmmo = MaxAmmo;
    Range = 1500.0f;

    PelletCount = 6; // 한 번에 발사될 탄환 수
    PelletSpread = 5.0f; // 탄환 퍼짐 정도
    ReloadTime = 2.5f;
}

void AShotgun::Fire()
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

                for (int32 i = 0; i < PelletCount; i++)
                {
                    // 탄환 퍼짐 적용
                    float SpreadYaw = FMath::RandRange(-PelletSpread, PelletSpread);
                    float SpreadPitch = FMath::RandRange(-PelletSpread, PelletSpread);
                    FRotator AdjustedRot = MuzzleRot + FRotator(SpreadPitch, SpreadYaw, 0);

                    ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, AdjustedRot);
                    if (SpawnedBullet)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("샷건 탄환 스폰 성공!"));
                    }
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
}

void AShotgun::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("샷건 재장전!"));
    CurrentAmmo = MaxAmmo;
}
