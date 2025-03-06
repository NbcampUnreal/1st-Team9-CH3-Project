#include "Pistol.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "DrawDebugHelpers.h"
#include "PlayerCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Camera/CameraComponent.h"

APistol::APistol()
{
    Damage = 15.0f;
    FireRate = 0.7f;  
    MaxAmmo = 12;
    CurrentAmmo = MaxAmmo;
    Range = 2000.0f;
    bIsAutomatic = false;
    BulletSpread = 1.0f;

    GunType = EGunType::PISTOL;
    bCanFire = true;  // 🔹 처음에는 발사가 가능해야 함.

    static ConstructorHelpers::FClassFinder<ABullet> BulletBP(TEXT("/Game/Items/Blueprints/BP_Bullet.BP_Bullet_C"));
    if (BulletBP.Succeeded())
    {
        BulletFactory = BulletBP.Class;
        UE_LOG(LogTemp, Warning, TEXT(" Bullet Factory 자동 설정 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" Bullet Factory 자동 설정 실패! 블루프린트 경로 확인 필요."));
    }
    bulletSound = LoadObject<USoundBase>(GetTransientPackage(), TEXT("/Game/Items/Sci-Fi_Shots_Pack2_Game_Of_Weapons/Wave/SciFi_Shot_P2__147_.SciFi_Shot_P2__147_"));
    if (bulletSound)
    {
        UE_LOG(LogTemp, Warning, TEXT("총소리 로드 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("총소리 로드 실패! 경로 확인 필요"));
    }


    MuzzleFlash = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Items/Particles/P_Gunshot.P_Gunshot"));
    if (MuzzleFlash)
    {
        UE_LOG(LogTemp, Warning, TEXT("머즐 플래시 로드 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("머즐 플래시 로드 실패! 경로 확인 필요"));
    }

    ImpactEffect = LoadObject<UParticleSystem>(GetTransientPackage(), TEXT("/Game/Items/Effects/ParticleSystems/Weapons/AssaultRifle/Impacts/P_AssaultRifle_IH.P_AssaultRifle_IH"));
    if (ImpactEffect)
    {
        UE_LOG(LogTemp, Warning, TEXT("피격 이펙트 로드 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("피격 이펙트 로드 실패! 경로 확인 필요"));
    }




}

void APistol::Reload()
{
    Super::Reload();
}

void APistol::Fire()
{
    if (!bCanFire)
    {
        UE_LOG(LogTemp, Warning, TEXT("피스톨 재사용 대기 중..."));
        return;
    }

    if (CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음! 현재 탄약: %d"), CurrentAmmo);
        return;
    }

    bCanFire = false;
    CurrentAmmo--;

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World 없음!"));
        return;
    }

    APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("플레이어 컨트롤러 없음!"));
        return;
    }


    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector TraceStart = CameraLocation;
    FVector ShotDirection = CameraRotation.Vector();
    FVector TraceEnd = TraceStart + (ShotDirection * 10000.0f); 

    FHitResult CrosshairHit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.bTraceComplex = true;

    bool bHitCrosshair = World->LineTraceSingleByChannel(CrosshairHit, TraceStart, TraceEnd, ECC_Pawn, QueryParams);

    FVector FinalTarget;
    if (bHitCrosshair)
    {
        FinalTarget = CrosshairHit.ImpactPoint;
    }
    else
    {
        FinalTarget = TraceEnd;
    }


    FVector MuzzlePos = MuzzleLocation ? MuzzleLocation->GetComponentLocation() : GetActorLocation();
    FVector AdjustedDirection = (FinalTarget - MuzzlePos).GetSafeNormal();
    FVector MuzzleTraceEnd = MuzzlePos + (AdjustedDirection * Range);

    FHitResult BulletHit;
    bool bHit = World->LineTraceSingleByChannel(BulletHit, MuzzlePos, MuzzleTraceEnd, ECC_Pawn, QueryParams);

    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, MuzzlePos, CameraRotation);
    }
    if (bulletSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, bulletSound, BulletHit.ImpactPoint);
    }

    if (bHit)
    {
        AActor* HitActor = BulletHit.GetActor();
        if (HitActor)
        {
            UGameplayStatics::ApplyDamage(
                HitActor,
                Damage,
                GetOwner()->GetInstigatorController(),
                this,
                nullptr
            );

            if (ImpactEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(World, ImpactEffect, BulletHit.ImpactPoint, FRotator::ZeroRotator);
            }
        }
    }


    GetWorld()->GetTimerManager().SetTimer(FireDelayTimer, this, &APistol::ResetFireCooldown, FireRate, false);

}


void APistol::ResetFireCooldown()
{
    bCanFire = true;
}


