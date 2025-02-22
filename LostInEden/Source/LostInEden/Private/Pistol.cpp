#include "Pistol.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

APistol::APistol()
{
    Damage = 25.0f; // 🔹 라이플보다 강하지만 샷건보단 약함
    FireRate = 0.3f; // 🔹 연사 속도 조절 (느리게)
    MaxAmmo = 12;
    CurrentAmmo = MaxAmmo;
    Range = 2000.0f; // 🔹 샷건보다는 길고 라이플보다는 짧음
    bIsAutomatic = false; // 🔹 단발 모드 (자동 연사 없음)
    BulletSpread = 1.0f; // 🔹 정확도 높음 (퍼짐 거의 없음)
}

void APistol::Fire()
{
    if (CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
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

    // 🔹 총기 메쉬에서 Muzzle 소켓 위치 가져오기
    USkeletalMeshComponent* MeshComponent = FindComponentByClass<USkeletalMeshComponent>();
    if (!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("MeshComponent가 없습니다! 블루프린트에서 확인하세요."));
        return;
    }

    FVector MuzzlePos = MeshComponent->GetSocketLocation("Muzzle");
    FRotator MuzzleRot = MeshComponent->GetSocketRotation("Muzzle");

    // ✅ 총구의 방향을 그대로 사용
    FVector ShotDirection = MuzzleRot.Vector();

    // 🔹 라인트레이스 시작점 및 종료점 설정 (Muzzle 방향 그대로 사용)
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

    // 🔹 디버그 로그 확인
    UE_LOG(LogTemp, Warning, TEXT("MuzzlePos: %s"), *MuzzlePos.ToString());
    UE_LOG(LogTemp, Warning, TEXT("ShotDirection: %s"), *ShotDirection.ToString());

    // 🔹 총알을 총구에서 발사
    ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
    if (SpawnedBullet)
    {
        CurrentAmmo--;
        UE_LOG(LogTemp, Warning, TEXT("총알 발사 성공! 남은 탄약: %d"), CurrentAmmo);
    }
}






void APistol::Reload()
{
    if (CurrentAmmo >= MaxAmmo)
    {
        UE_LOG(LogTemp, Warning, TEXT("이미 탄창이 가득 찼음!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("권총 재장전!"));
    CurrentAmmo = MaxAmmo;
}
