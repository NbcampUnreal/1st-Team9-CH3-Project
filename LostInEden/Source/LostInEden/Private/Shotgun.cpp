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
        QueryParams.AddIgnoredActor(GetOwner()); // 플레이어도 무시

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
                UE_LOG(LogTemp, Warning, TEXT("라인 트레이스 적중! 타겟: %s"), *HitActor->GetName());

                UGameplayStatics::ApplyDamage(
                    HitActor,
                    Damage,
                    GetOwner()->GetInstigatorController(),
                    this,
                    nullptr
                );

                UE_LOG(LogTemp, Warning, TEXT("샷건이 %s에 명중! 피해량: %f"), *HitActor->GetName(), Damage);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("라인 트레이스 충돌했으나 타겟이 없음!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("라인 트레이스 미적중!"));
        }

        // 충돌 설정을 추가하여 총알끼리 겹쳐도 문제없도록 처리
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation(), SpawnParams);
        if (SpawnedBullet)
        {
            // 총알끼리 충돌하지 않도록 충돌 비활성화
            SpawnedBullet->SetActorEnableCollision(false);

            // 충돌 컴포넌트가 있다면, 총알끼리 충돌하지 않도록 설정
            UPrimitiveComponent* BulletCollision = Cast<UPrimitiveComponent>(SpawnedBullet->GetRootComponent());
            if (BulletCollision)
            {
                BulletCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
                BulletCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
                BulletCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
                BulletCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // 사용자 지정 채널
                BulletCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore); // 샷건 총알끼리 충돌하지 않음
            }

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