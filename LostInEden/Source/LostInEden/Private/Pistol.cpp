#include "Pistol.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "DrawDebugHelpers.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"

APistol::APistol()
{
    static ConstructorHelpers::FClassFinder<ABullet> BulletBP(TEXT("/Game/Items/Blueprints/BP_Bullet.BP_Bullet"));

    if (BulletBP.Succeeded())
    {
        BulletFactory = BulletBP.Class;
        UE_LOG(LogTemp, Warning, TEXT("Pistol 생성자: Bullet Factory 자동 설정 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Pistol 생성자: Bullet Factory 자동 설정 실패! 블루프린트 경로 확인 필요."));
    }

    Damage = 20.0f;
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
    AutoAssignBulletFactory();
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
        UE_LOG(LogTemp, Error, TEXT("Pistol: Bullet Factory가 설정되지 않음! 자동 할당 시도."));
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
    FVector TraceEnd = TraceStart + (ShotDirection * Range);

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

            UE_LOG(LogTemp, Warning, TEXT("권총이 %s에 명중! 피해량: %f"), *HitActor->GetName(), AppliedDamage);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("트레이스 미적중!"));
    }

    
    ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
    if (SpawnedBullet)
    {
        UE_LOG(LogTemp, Warning, TEXT("총알 스폰 성공!"));
    }
    Super::Fire();
}



void APistol::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("권총은 무한 탄창이므로 재장전이 필요 없음!"));
}

void APistol::AutoAssignBulletFactory()
{
    if (BulletFactory) return;

    FString BulletPath = TEXT("/Game/Items/Blueprints/BP_Bullet.BP_Bullet");
    UClass* BulletClass = LoadClass<ABullet>(nullptr, *BulletPath);

    if (BulletClass)
    {
        BulletFactory = BulletClass;
        UE_LOG(LogTemp, Warning, TEXT("Pistol: Bullet Factory 자동 설정 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Pistol: Bullet Factory 자동 설정 실패! 블루프린트 경로 확인 필요."));
    }
}
