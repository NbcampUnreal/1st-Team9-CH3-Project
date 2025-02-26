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

    if (CurrentAmmo < PelletCount)
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

        TArray<FHitResult> HitResults;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        QueryParams.AddIgnoredActor(GetOwner()); // 플레이어 무시
        QueryParams.bTraceComplex = true;  // 복잡한 충돌 검사 허용 (적이 겹칠 때 정확한 감지)

        bool bHit = World->SweepMultiByChannel(
            HitResults,
            TraceStart,
            TraceEnd,
            FQuat::Identity,
            ECC_Pawn,
            FCollisionShape::MakeSphere(15.0f), // ← 구체 크기 설정 (값을 조절해보세요)
            QueryParams
        );

        DrawDebugLine(World, TraceStart, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 5.0f, 0, 5.0f);

        if (bHit)
        {
            TSet<AActor*> DamagedActors;  // 중복 데미지 방지를 위한 집합

            for (const FHitResult& HitResult : HitResults)
            {
                AActor* HitActor = HitResult.GetActor();
                if (HitActor && !DamagedActors.Contains(HitActor))
                {
                    UE_LOG(LogTemp, Warning, TEXT("트레이스 명중! 맞은 대상: %s"), *HitActor->GetName());

                    // 거리 기반 데미지 계산
                    float Distance = FVector::Dist(MuzzlePos, HitResult.ImpactPoint);
                    float MinRange = 100.0f;
                    float MaxRange = Range;
                    float DamageMultiplier = 1.0f - FMath::Clamp((Distance - MinRange) / (MaxRange - MinRange), 0.0f, 1.0f);

                    float FinalDamage = Damage * DamageMultiplier;

                    // ApplyDamage 호출
                    float AppliedDamage = UGameplayStatics::ApplyDamage(
                        HitActor,
                        FinalDamage,
                        GetOwner()->GetInstigatorController(),
                        this,
                        nullptr
                    );

                    UE_LOG(LogTemp, Warning, TEXT("샷건이 %s에 명중! 피해량: %f (거리: %f)"), *HitActor->GetName(), AppliedDamage, Distance);

                    // 중복 공격 방지
                    DamagedActors.Add(HitActor);

                    // 충돌 지점 디버깅
                    DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 8, FColor::Yellow, false, 5.0f);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("트레이스 미적중!"));
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