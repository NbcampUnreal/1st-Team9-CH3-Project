#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = true;

    // 총 본체 Mesh 생성
    GunStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunStaticMesh"));
    SetRootComponent(GunStaticMesh);

    // MuzzleLocation은 자식 클래스에서 직접 생성할 예정
    BulletFactory = nullptr;

    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
}

int32 AGun::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int32 AGun::GetMaxAmmo() const
{
    return MaxAmmo;
}

void AGun::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("AGun::Fire() 호출됨 - 자식 클래스에서 오버라이드 필요!"));
}


void AGun::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("AGun 기본 Reload() 실행됨"));
    CurrentAmmo = MaxAmmo;
}

void AGun::BeginPlay()
{
    Super::BeginPlay();

    // BulletFactory 자동 설정
    AutoAssignBulletFactory();
}

void AGun::AutoAssignBulletFactory()
{
    if (BulletFactory)
    {
        return; // 이미 설정되어 있으면 변경하지 않음
    }

    // 🔥 블루프린트 경로 수정 ('.BP_Bullet' 대신 '.BP_Bullet_C' 사용)
    static ConstructorHelpers::FClassFinder<ABullet> BulletBP(TEXT("/Game/Items/Blueprints/BP_Bullet.BP_Bullet_C"));

    if (BulletBP.Succeeded())
    {
        BulletFactory = BulletBP.Class;
        UE_LOG(LogTemp, Warning, TEXT("Bullet Factory 자동 설정 완료! BP_Bullet이 설정되었습니다."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet Factory 자동 설정 실패! 블루프린트 경로를 확인하세요."));
        BulletFactory = nullptr;
    }
}
