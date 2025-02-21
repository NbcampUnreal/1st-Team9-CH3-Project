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
    if (CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 탄약 없음! 현재 탄약: %d"), CurrentAmmo);
        return;
    }

    if (!BulletFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음! 블루프린트에서 확인하세요."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World가 없음!"));
        return;
    }

    if (!MuzzleLocation)
    {
        UE_LOG(LogTemp, Error, TEXT("MuzzleLocation이 설정되지 않음! 블루프린트에서 확인하세요."));
        return;
    }

    CurrentAmmo--;
    FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
    FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();

    int32 SpawnedPellets = 0;
    for (int32 i = 0; i < PelletCount; i++)
    {
        // 탄환 퍼짐 적용
        float SpreadYaw = FMath::RandRange(-PelletSpread, PelletSpread);
        float SpreadPitch = FMath::RandRange(-PelletSpread, PelletSpread);
        FRotator AdjustedRot = MuzzleRot + FRotator(SpreadPitch, SpreadYaw, 0);

        ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, AdjustedRot);
        if (SpawnedBullet)
        {
            SpawnedPellets++;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("샷건 탄환 스폰 실패! BulletFactory 또는 MuzzlePos 확인 필요"));
        }
    }

    if (SpawnedPellets > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 탄환 %d개 스폰 성공! 남은 탄약: %d"), SpawnedPellets, CurrentAmmo);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("샷건 탄환이 하나도 생성되지 않음! 블루프린트 설정 확인 필요"));
    }
}

void AShotgun::Reload()
{
    if (CurrentAmmo >= MaxAmmo)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 이미 탄창 가득 참!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("샷건 재장전! 탄약: %d -> %d"), CurrentAmmo, MaxAmmo);
    CurrentAmmo = MaxAmmo;
}

void AShotgun::BeginPlay()
{
    Super::BeginPlay();

    Reload(); // ✅ 게임 시작 시 탄창 채우기

    UE_LOG(LogTemp, Warning, TEXT("샷건 시작! 현재 탄약: %d"), CurrentAmmo);
}
