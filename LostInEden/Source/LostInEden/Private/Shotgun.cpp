#include "Shotgun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

AShotgun::AShotgun()
{
    Damage = 30.0f; // ✅ 샷건 탄환 개별 데미지 설정
    FireRate = 2.0f; // ✅ 발사 후 딜레이 증가
    MaxAmmo = 20;
    CurrentAmmo = MaxAmmo;
    Range = 500.0f; // ✅ 발사 범위를 줄여 근거리 공격에 최적화
    ReloadTime = 3.0f; // ✅ 재장전 시간이 길도록 설정
    PelletCount = 8; // ✅ 탄환 개수 증가
    PelletSpread = 8.0f; // ✅ 탄환 퍼짐 정도를 줄여 더 좁은 범위에서 발사

    bCanFire = true; // ✅ 연속 발사 방지용 변수 초기화
}

void AShotgun::Fire()
{
    if (!bCanFire)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 재사용 대기 중..."));
        return;
    }

    if (CurrentAmmo <= 0) // ✅ 탄약이 없으면 발사 불가
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 탄약 없음! 현재 탄약: %d"), CurrentAmmo);
        return;
    }

    bCanFire = false; // ✅ 발사 후 딜레이 적용

    int32 NumShots = FMath::Min(CurrentAmmo, PelletCount); // ✅ 남은 탄약 개수까지만 발사
    CurrentAmmo -= NumShots; // ✅ 발사한 개수만큼 탄약 차감

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

    ACharacter* Player = Cast<ACharacter>(GetOwner());
    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("소유자가 캐릭터가 아닙니다!"));
        return;
    }

    USkeletalMeshComponent* MeshComponent = FindComponentByClass<USkeletalMeshComponent>();
    if (!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("MeshComponent가 없습니다! 블루프린트에서 확인하세요."));
        return;
    }

    FVector MuzzlePos = MeshComponent->GetSocketLocation("Muzzle");
    FRotator MuzzleRot = MeshComponent->GetSocketRotation("Muzzle");

    // ✅ 좁은 범위에서 여러 탄환이 퍼지는 방식으로 조정
    for (int32 i = 0; i < NumShots; i++) // ✅ PelletCount 개수만큼 발사
    {
        float SpreadYaw = FMath::RandRange(-PelletSpread * 0.5f, PelletSpread * 0.5f); // ✅ 퍼짐 범위를 더 좁게 조정
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
    bCanFire = true; // ✅ 재장전 완료 후 발사 가능하도록 설정
    UE_LOG(LogTemp, Warning, TEXT("샷건 재장전 완료! 탄약: %d"), CurrentAmmo);
}

void AShotgun::BeginPlay()
{
    Super::BeginPlay();
    Reload();
    UE_LOG(LogTemp, Warning, TEXT("샷건 시작! 현재 탄약: %d"), CurrentAmmo);
}