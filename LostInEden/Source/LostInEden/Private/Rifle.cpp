#include "Rifle.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

ARifle::ARifle()
{
    static ConstructorHelpers::FClassFinder<AActor> BulletBP(TEXT("/Game/Items/Blueprints/BP_Bullet.BP_Bullet_C"));

    if (BulletBP.Succeeded())
    {
        BulletFactory = BulletBP.Class;
        UE_LOG(LogTemp, Warning, TEXT("Rifle 생성자: Bullet Factory 자동 설정 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Rifle 생성자: Bullet Factory 자동 설정 실패! 블루프린트 경로 확인 필요."));
    }

    if (!GunStaticMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("GunStaticMesh가 nullptr입니다! MuzzleLocation을 연결할 수 없습니다."));
    }
    else if (!MuzzleLocation)
    {
        // 🔥 중복 생성 방지
        MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
        MuzzleLocation->SetupAttachment(GunStaticMesh);
        MuzzleLocation->SetRelativeLocation(FVector(0.f, 40.f, 15.f));
        UE_LOG(LogTemp, Warning, TEXT("MuzzleLocation 생성 완료!"));
    }

    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
    bIsAutomatic = true;
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
        UE_LOG(LogTemp, Error, TEXT("Rifle: Bullet Factory가 설정되지 않음! 자동 할당 시도."));
        AutoAssignBulletFactory();
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
    FVector TraceEnd = TraceStart + (ShotDirection * 10000.0f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = World->LineTraceSingleByChannel(
        HitResult, TraceStart, TraceEnd, ECC_Pawn, QueryParams);

    DrawDebugLine(World, TraceStart, TraceEnd, FColor::Green, false, 5.0f, 0, 5.0f);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("트레이스 명중! 맞은 대상: %s"), *HitActor->GetName());

            
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

    // 🔹 총알 스폰
    ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
    if (SpawnedBullet)
    {
        UE_LOG(LogTemp, Warning, TEXT("총알 스폰 성공!"));
    }
    Super::Fire();
}



void ARifle::StartAutoFire()
{
    if (CurrentAmmo > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("자동사격 남은 탄약: %d"), CurrentAmmo);
        CurrentAmmo--;

        if (!BulletFactory)
        {
            UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음!"));
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
        World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, MuzzleRot);

        GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ARifle::StartAutoFire, FireRate, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
    }
}

void ARifle::StopAutoFire()
{
    GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);
}

void ARifle::BurstFire()
{
    for (int32 i = 0; i < BurstCount; i++)
    {
        if (CurrentAmmo > 0)
        {
            CurrentAmmo--;
            UE_LOG(LogTemp, Warning, TEXT("점사 사격: %d/%d"), i + 1, BurstCount);
        }
        else
        {
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

void ARifle::AutoAssignBulletFactory()
{
    if (BulletFactory) return;

    FString BulletPath = TEXT("/Game/Items/Blueprints/BP_Bullet.BP_Bullet");
    UClass* BulletClass = LoadClass<ABullet>(nullptr, *BulletPath);

    if (BulletClass)
    {
        BulletFactory = BulletClass;
        UE_LOG(LogTemp, Warning, TEXT("Rifle: Bullet Factory 자동 설정 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Rifle: Bullet Factory 자동 설정 실패! 블루프린트 경로 확인 필요."));
    }
}