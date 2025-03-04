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
    if (!bCanFire || CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("발사 대기 중..."));
        return;
    }

    bCanFire = false;  

    if (!BulletFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음! 블루프린트에서 확인하세요."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World 없음!"));
        return;
    }

    if (!MuzzleLocation)
    {
        MuzzleLocation = GunStaticMesh;
    }

    FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
    FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
    FVector ShotDirection = MuzzleRot.Vector();

    FVector TraceStart = MuzzlePos;
    FVector TraceEnd = TraceStart + (ShotDirection * Range);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());

    bool bHit = World->LineTraceSingleByChannel(
        HitResult, TraceStart, TraceEnd, ECC_Pawn, QueryParams);

    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, MuzzlePos, MuzzleRot);
    }

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            if (bulletSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, bulletSound, HitResult.Location);
            }

            if (ImpactEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(World, ImpactEffect, HitResult.Location, FRotator::ZeroRotator);
            }

            float AppliedDamage = UGameplayStatics::ApplyDamage(
                HitActor,
                Damage,
                GetOwner()->GetInstigatorController(),
                this,
                nullptr
            );
        }
    }


    if (CurrentAmmo > 0)
    {
        CurrentAmmo--;
    }

    GetWorld()->GetTimerManager().SetTimer(FireCooldownTimer, this, &APistol::ResetFireCooldown, FireRate, false);
}

void APistol::ResetFireCooldown()
{
    bCanFire = true;
}


