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
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
        return;
    }

    CurrentAmmo--;

    if (!BulletFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("Rifle: Bullet Factory가 설정되지 않음!"));
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
        UE_LOG(LogTemp, Error, TEXT("Gun: MuzzleLocation이 nullptr입니다! GunStaticMesh를 사용합니다."));
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
            UE_LOG(LogTemp, Warning, TEXT("트레이스 명중! 맞은 대상: %s"), *HitActor->GetName());

            // 🔹 피격 사운드 및 이펙트
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

            UE_LOG(LogTemp, Warning, TEXT("소총이 %s에 명중! 피해량: %f"), *HitActor->GetName(), AppliedDamage);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("트레이스 미적중!"));
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
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
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
            UE_LOG(LogTemp, Warning, TEXT("점사 사격: %d/%d"), i + 1, BurstCount);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("탄약 없음! 점사 중지."));
            break;
        }
    }
}


void ARifle::Reload()
{
    
    if (CurrentAmmo >= MaxAmmo)
    {
        UE_LOG(LogTemp, Warning, TEXT("이미 탄창이 가득 찼음!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("소총 재장전!"));
    CurrentAmmo = MaxAmmo;
}

