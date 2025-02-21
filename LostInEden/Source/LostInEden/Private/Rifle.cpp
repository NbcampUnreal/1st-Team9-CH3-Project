#include "Rifle.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

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
                FVector ShotDirection = MuzzleRot.Vector(); // 총구의 정면 방향

                // 🔹 라인트레이스 설정
                FVector TraceStart = MuzzlePos;
                FVector TraceEnd = TraceStart + (ShotDirection * 10000.0f); // 10,000cm(100m) 거리

                FHitResult HitResult;
                FCollisionQueryParams QueryParams;
                QueryParams.AddIgnoredActor(this); // 캐릭터 자신 제외

                if (this->EquippedGun)  // 🔹 명시적으로 this 사용
                {
                    QueryParams.AddIgnoredActor(Cast<AActor>(this->EquippedGun));
                }


                // 🔹 라인트레이스 실행
                bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

                // 라인트레이스 디버그 (편의상 추가, 나중에 제거 가능)
                DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 1.0f);

                if (bHit)
                {
                    // 맞은 대상 위치를 출력
                    UE_LOG(LogTemp, Warning, TEXT("총알이 %s 에 맞음!"), *HitResult.GetActor()->GetName());


                    // 맞은 지점으로 총알 발사 방향 조정
                    ShotDirection = (HitResult.ImpactPoint - MuzzlePos).GetSafeNormal();
                }

                // 🔹 총알 스폰
                ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
                if (SpawnedBullet)
                {
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
}



void ARifle::StartAutoFire()
{
    if (CurrentAmmo > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("자동사격 남은 탄약: %d"), CurrentAmmo);
        CurrentAmmo--;

        if (BulletFactory)
        {
            UWorld* World = GetWorld();
            if (World)
            {
                FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
                FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
                World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, MuzzleRot);
            }
        }

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
    UE_LOG(LogTemp, Warning, TEXT("소총 재장전!"));
    CurrentAmmo = MaxAmmo;
}