#include "Shotgun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"

#include "Camera/CameraComponent.h"

AShotgun::AShotgun()
{
    Damage = 45.0f;
    FireRate = 2.0f;
    MaxAmmo = 20;
    CurrentAmmo = MaxAmmo;
    Range = 500.0f;
    ReloadTime = 3.0f;
    PelletCount = 8;
    PelletSpread = 8.0f;

    GunType = EGunType::SHOTGUN;

    bCanFire = true;

    static ConstructorHelpers::FClassFinder<ABullet> BulletBP(TEXT("/Game/Items/Blueprints/BP_ShotGunBullet.BP_ShotGunBullet_C"));
    if (BulletBP.Succeeded())
    {
        BulletFactory = BulletBP.Class;
        UE_LOG(LogTemp, Warning, TEXT(" Bullet Factory 자동 설정 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" Bullet Factory 자동 설정 실패! 블루프린트 경로 확인 필요."));
    }

    bulletSound = LoadObject<USoundBase>(GetTransientPackage(), TEXT("/Game/Items/Sci-Fi_Shots_Pack2_Game_Of_Weapons/Wave/SciFi_Shot_P2__139_.SciFi_Shot_P2__139_"));
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

    EmptyMagSound = LoadObject<USoundBase>(GetTransientPackage(), TEXT("/Game/Items/Sci-Fi_Shots_Pack2_Game_Of_Weapons/Wave/SciFi_Shot_P2__96_.SciFi_Shot_P2__96_"));
    if (EmptyMagSound)
    {
        UE_LOG(LogTemp, Warning, TEXT("빈 탄창 사운드 로드 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("빈 탄창 사운드 로드 실패! 경로 확인 필요"));
    }
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
        if (EmptyMagSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, EmptyMagSound, GetActorLocation());
        }
        return;
    }

    bCanFire = false;
    int32 NumShots = FMath::Min(CurrentAmmo, PelletCount);
    CurrentAmmo -= NumShots;

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
    FVector TraceEnd = TraceStart + (ShotDirection * 1000.0f); 

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

    if (!MuzzleLocation)
    {
        UE_LOG(LogTemp, Error, TEXT("Gun: MuzzleLocation이 nullptr입니다! GunStaticMesh를 사용합니다."));
        MuzzleLocation = GunStaticMesh;
    }

    FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
    FVector BaseDirection = (FinalTarget - MuzzlePos).GetSafeNormal(); 

    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, MuzzlePos, CameraRotation);
    }

    TSet<AActor*> DamagedActors;

    for (int32 i = 0; i < NumShots; i++)
    {
        float SpreadYaw = FMath::RandRange(-PelletSpread * 0.5f, PelletSpread * 0.5f);
        float SpreadPitch = FMath::RandRange(-PelletSpread * 0.5f, PelletSpread * 0.5f);

        FRotator AdjustedRot = BaseDirection.Rotation();
        AdjustedRot.Yaw += SpreadYaw;
        AdjustedRot.Pitch += SpreadPitch;

        FVector AdjustedDirection = AdjustedRot.Vector();
        FVector MuzzleTraceEnd = MuzzlePos + (AdjustedDirection * Range);

        TArray<FHitResult> HitResults;
        bool bHit = World->LineTraceMultiByChannel(HitResults, MuzzlePos, MuzzleTraceEnd, ECC_Pawn, QueryParams);

        if (bHit)
        {
            for (const FHitResult& HitResult : HitResults)
            {
                AActor* HitActor = HitResult.GetActor();
                if (HitActor && !DamagedActors.Contains(HitActor))
                {
                    float Distance = FVector::Dist(MuzzlePos, HitResult.ImpactPoint);
                    float DamageMultiplier = 1.0f - FMath::Clamp((Distance - 100.0f) / (Range - 100.0f), 0.0f, 1.0f);
                    float FinalDamage = Damage * DamageMultiplier;

                    UGameplayStatics::ApplyDamage(
                        HitActor,
                        FinalDamage,
                        GetOwner()->GetInstigatorController(),
                        this,
                        nullptr
                    );

                    DamagedActors.Add(HitActor);

                    if (ImpactEffect)
                    {
                        UGameplayStatics::SpawnEmitterAtLocation(World, ImpactEffect, HitResult.ImpactPoint, FRotator::ZeroRotator);
                    }

                    if (bulletSound)
                    {
                        UGameplayStatics::PlaySoundAtLocation(this, bulletSound, HitResult.ImpactPoint);
                    }
                }
            }
        }
    }

    GetWorld()->GetTimerManager().SetTimer(FireDelayTimer, this, &AShotgun::ResetFire, FireRate, false);
}


void AShotgun::ResetFire()
{
    bCanFire = true;
    
}

void AShotgun::Reload()
{
  
    Super::Reload();

}


void AShotgun::BeginPlay()
{
    Super::BeginPlay();
    Reload();
    
}