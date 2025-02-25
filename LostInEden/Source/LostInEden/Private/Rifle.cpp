#include "Rifle.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

ARifle::ARifle()
{
    Damage = 35.0f;
    FireRate = 0.1f;
    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
    Range = 3000.0f;

    bIsAutomatic = true;
    BulletSpread = 2.0f;
    BurstCount = 3;
    BurstFireRate = 0.15f;
}

void ARifle::Fire()
{
    if (CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
        return;
    }

    CurrentAmmo--;

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

    FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
    FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
    FVector ShotDirection = MuzzleRot.Vector();

    FVector TraceStart = MuzzlePos;
    FVector TraceEnd = TraceStart + (ShotDirection * 10000.0f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = World->LineTraceSingleByChannel(
        HitResult, TraceStart, TraceEnd, ECC_Pawn, QueryParams); // 🔹 ECC_Pawn 사용

    DrawDebugLine(World, TraceStart, TraceEnd, FColor::Green, false, 5.0f, 0, 5.0f); // 🔹 트레이스 확인

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("트레이스 명중! 맞은 대상: %s"), *HitActor->GetName());

            // 🔹 ApplyDamage 실행
            float AppliedDamage = UGameplayStatics::ApplyDamage(
                HitActor,
                Damage,
                GetOwner()->GetInstigatorController(),
                this,
                nullptr
            );

            UE_LOG(LogTemp, Warning, TEXT("샷건이 %s에 명중! 피해량: %f"), *HitActor->GetName(), AppliedDamage);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("트레이스 미적중!"));
    }

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UGameplayStatics::ApplyDamage(
                HitActor,
                Damage,
                GetOwner()->GetInstigatorController(),
                this,
                nullptr
            );

            UE_LOG(LogTemp, Warning, TEXT("소총이 %s에 명중! 피해량: %f"), *HitActor->GetName(), Damage);
        }
    }

    // 🔹 총알 스폰
    ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
    if (SpawnedBullet)
    {
        UE_LOG(LogTemp, Warning, TEXT("총알 스폰 성공!"));
    }
}


void ARifle::StartAutoFire()
{
    if (CurrentAmmo > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("자동사격 남은 탄약: %d"), CurrentAmmo);
        CurrentAmmo--;

        if (!BulletFactory)
        {
            UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음!"));
            return;
        }

        UWorld* World = GetWorld();
        if (!World || !MuzzleLocation)
        {
            UE_LOG(LogTemp, Error, TEXT("World 또는 MuzzleLocation이 없음!"));
            return;
        }

        FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
        FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
        World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, MuzzleRot);

        GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ARifle::StartAutoFire, FireRate, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
    }
}

void ARifle::StopAutoFire()
{
    GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);
}

void ARifle::BurstFire()
{
    for (int32 i = 0; i < BurstCount; i++)
    {
        if (CurrentAmmo > 0)
        {
            CurrentAmmo--;
            UE_LOG(LogTemp, Warning, TEXT("점사 사격: %d/%d"), i + 1, BurstCount);
        }
        else
        {
            break;
        }
    }
}

void ARifle::Reload()
{
    // ✅ 탄창이 가득 차 있으면 재장전 불필요
    if (CurrentAmmo >= MaxAmmo)
    {
        UE_LOG(LogTemp, Warning, TEXT("이미 탄창이 가득 찼음!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("소총 재장전!"));
    CurrentAmmo = MaxAmmo;
}