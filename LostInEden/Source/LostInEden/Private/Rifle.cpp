#include "Rifle.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystem.h"


ARifle::ARifle()
{

    Damage = 35.0f;
    FireRate = 0.1f;
    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
    Range = 3000.0f;

    bIsAutomatic = false;
    BulletSpread = 2.0f;
    BurstCount = 3;
    BurstFireRate = 0.15f;

    GunType = EGunType::RIFLE;

    static ConstructorHelpers::FClassFinder<ABullet> BulletBP(TEXT("/Game/Items/Blueprints/BP_BulletRifle.BP_BulletRifle_C"));
    if (BulletBP.Succeeded())
    {
        BulletFactory = BulletBP.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" Bullet Factory 자동 설정 실패! 블루프린트 경로 확인 필요."));
    }

    bulletSound = LoadObject<USoundBase>(GetTransientPackage(), TEXT("/Game/Items/Sci-Fi_Shots_Pack2_Game_Of_Weapons/Wave/SciFi_Shot_P2__66_.SciFi_Shot_P2__66_"));
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


void ARifle::Fire()
{
    if (CurrentAmmo <= 0)
    {
        return;
    }

    CurrentAmmo--;

    if (!BulletFactory)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
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
    else
    {

    }
}


void ARifle::StartAutoFire()
{
    if (CurrentAmmo > 0)
    {
        Fire();  

        GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ARifle::StartAutoFire, FireRate, true);
        bIsAutomatic = true;
    }
    else
    {
        StopAutoFire(); 
    }
}


void ARifle::StopAutoFire()
{
    if (bIsAutomatic)
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);
        bIsAutomatic = false;
    }
}


void ARifle::BurstFire()
{
    for (int32 i = 0; i < BurstCount; i++)
    {
        if (CurrentAmmo > 0)
        {
            Fire();  
        }
        else
        {
            break;
        }
    }
}


void ARifle::Reload()
{
    
    Super::Reload();
}

