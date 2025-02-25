#include "Pistol.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "DrawDebugHelpers.h"
#include "PlayerCharacter.h"
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

    /*APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Pistol: 플레이어 캐릭터를 찾을 수 없습니다!"));
        return;
    }*/

    // 🔹 총을 캐릭터 손에 부착
    /*FName WeaponSocket = "GunSocket_R";
    AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);*/

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

    FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
    FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
    FVector ShotDirection = MuzzleRot.Vector();

    FVector TraceStart = MuzzlePos;
    FVector TraceEnd = TraceStart + (ShotDirection * Range);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
    DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 1.0f);

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

            UE_LOG(LogTemp, Warning, TEXT("권총이 %s에 명중! 피해량: %f"), *HitActor->GetName(), Damage);
        }
    }

    ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
    if (SpawnedBullet)
    {
        UE_LOG(LogTemp, Warning, TEXT("총알 스폰 성공!"));
    }
}


void APistol::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("권총은 무한 탄창이므로 재장전이 필요 없음!"));
}