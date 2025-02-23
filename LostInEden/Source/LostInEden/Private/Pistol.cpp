#include "Pistol.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

APistol::APistol()
{
    Damage = 15.0f;
    FireRate = 0.3f;
    MaxAmmo = 12;
    CurrentAmmo = MaxAmmo;
    Range = 2000.0f;
    bIsAutomatic = false;
    BulletSpread = 1.0f;

}
void APistol::BeginPlay()
{
    Super::BeginPlay();

    ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Pistol: 플레이어 캐릭터를 찾을 수 없습니다!"));
        return;
    }

    // 🔹 총을 캐릭터 손에 부착
    FName WeaponSocket = "GunSocket_R";
    AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);

    // 🔹 총 회전값을 보정 (Yaw 180도 회전)
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += 180.0f;
    SetActorRotation(NewRotation);

    UE_LOG(LogTemp, Warning, TEXT("%s가 플레이어 손에 올바르게 장착됨!"), *GetName());
}



void APistol::Fire()
{
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
    FVector ShotDirection = MuzzleRot.Vector();

    // 🔹 라인트레이스 시작점 및 종료점 설정
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
        UE_LOG(LogTemp, Warning, TEXT("총알 발사 성공! (무한 탄약)"));
    }
}

void APistol::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("권총은 무한 탄창이므로 재장전이 필요 없음!"));
}
