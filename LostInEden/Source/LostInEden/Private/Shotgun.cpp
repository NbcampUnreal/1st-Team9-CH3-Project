#include "Shotgun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

AShotgun::AShotgun()
{
    Damage = 30.0f;
    FireRate = 2.0f;
    MaxAmmo = 20;
    CurrentAmmo = MaxAmmo;
    Range = 500.0f;
    ReloadTime = 3.0f;
    PelletCount = 8;
    PelletSpread = 8.0f;

    bCanFire = true;
}

void AShotgun::Fire()
{
    if (!bCanFire)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 재사용 대기 중..."));
        return;
    }

    if (CurrentAmmo <= PelletCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 탄약 없음! 현재 탄약: %d"), CurrentAmmo);
        return;
    }

    bCanFire = false;

    int32 NumShots = FMath::Min(CurrentAmmo, PelletCount);
    CurrentAmmo -= NumShots;

    if (!BulletFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음! 블루프린트에서 확인하세요."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World || !MuzzleLocation)
    {
        UE_LOG(LogTemp, Error, TEXT("World 또는 MuzzleLocation이 없음!"));
        return;
    }

    // 🔹 MuzzleLocation을 사용하여 위치 및 방향 가져오기
    FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
    FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();

    for (int32 i = 0; i < NumShots; i++)
    {
        float SpreadYaw = FMath::RandRange(-PelletSpread * 0.5f, PelletSpread * 0.5f);
        float SpreadPitch = FMath::RandRange(-PelletSpread * 0.5f, PelletSpread * 0.5f);

        FRotator AdjustedRot = MuzzleRot;
        AdjustedRot.Yaw += SpreadYaw;
        AdjustedRot.Pitch += SpreadPitch;

        FVector ShotDirection = AdjustedRot.Vector();
        FVector TraceStart = MuzzlePos;
        FVector TraceEnd = TraceStart + (ShotDirection * Range);

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);

        bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
        DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 1.0f);

        if (bHit)
        {
            ShotDirection = (HitResult.ImpactPoint - MuzzlePos).GetSafeNormal();
        }

        ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
        if (SpawnedBullet)
        {
            UE_LOG(LogTemp, Warning, TEXT("샷건 탄환 스폰 성공!"));
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("샷건 발사 완료! 남은 탄약: %d"), CurrentAmmo);
    GetWorld()->GetTimerManager().SetTimer(FireDelayTimer, this, &AShotgun::ResetFire, FireRate, false);
}

void AShotgun::ResetFire()
{
    bCanFire = true;
    UE_LOG(LogTemp, Warning, TEXT("샷건 재사용 가능!"));
}

void AShotgun::Reload()
{
    if (CurrentAmmo >= MaxAmmo)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 이미 탄창 가득 참!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("샷건 재장전 중..."));
    GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
    GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AShotgun::FinishReload, ReloadTime, false);
}

void AShotgun::FinishReload()
{
    CurrentAmmo = MaxAmmo;
    bCanFire = true;
    UE_LOG(LogTemp, Warning, TEXT("샷건 재장전 완료! 탄약: %d"), CurrentAmmo);
}

void AShotgun::BeginPlay()
{
    Super::BeginPlay();
    Reload();
    UE_LOG(LogTemp, Warning, TEXT("샷건 시작! 현재 탄약: %d"), CurrentAmmo);
}
