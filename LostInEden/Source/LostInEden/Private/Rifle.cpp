#include "Rifle.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

ARifle::ARifle()
{
    Damage = 15.0f;
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
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World가 없음!"));
        return;
    }

    // 🔹 총기 메쉬에서 Muzzle 소켓 위치 가져오기
    USkeletalMeshComponent* MeshComponent = FindComponentByClass<USkeletalMeshComponent>();
    if (!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("MeshComponent가 없습니다! 블루프린트에서 확인하세요."));
        return;
    }

    FVector MuzzlePos = MeshComponent->GetSocketLocation("Muzzle");
    FRotator MuzzleRot = MeshComponent->GetSocketRotation("Muzzle");
    FVector ShotDirection = MuzzleRot.Vector();  // 🔹 총구 방향을 그대로 사용

    // 🔹 총구에서 라인트레이스 시작 (조준선)
    FVector TraceStart = MuzzlePos;
    FVector TraceEnd = TraceStart + (ShotDirection * 10000.0f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
    DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 1.0f);

    if (bHit)
    {
        ShotDirection = (HitResult.ImpactPoint - MuzzlePos).GetSafeNormal();
    }

    // 🔹 디버그 로그 확인
    UE_LOG(LogTemp, Warning, TEXT("MuzzlePos: %s"), *MuzzlePos.ToString());
    UE_LOG(LogTemp, Warning, TEXT("ShotDirection: %s"), *ShotDirection.ToString());

    // 🔹 총알을 총구에서 발사
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

