#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = true;

    GunStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunStaticMesh"));
    SetRootComponent(GunStaticMesh);

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(GunStaticMesh);

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
    if (CurrentAmmo > 0)
    {
        CurrentAmmo--;

        if (!BulletFactory)
        {
            UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음! BP_Bullet을 자동 할당합니다."));
            AutoAssignBulletFactory(); // Bullet Factory 자동 할당
        }

        if (BulletFactory)
        {
            UWorld* World = GetWorld();
            if (World)
            {
                FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
                FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
                FVector ShotDirection = MuzzleRot.Vector();

                ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
                if (SpawnedBullet)
                {
                    UProjectileMovementComponent* BulletMovement = SpawnedBullet->FindComponentByClass<UProjectileMovementComponent>();
                    if (BulletMovement)
                    {
                        BulletMovement->Velocity = ShotDirection * BulletMovement->InitialSpeed;
                    }

                    UE_LOG(LogTemp, Warning, TEXT("총알 스폰 성공!"));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Bullet Factory 자동 할당 실패! 블루프린트 설정을 확인하세요."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
    }
}

void AGun::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("재장전!"));
    CurrentAmmo = MaxAmmo;
}

void AGun::BeginPlay()
{
    Super::BeginPlay();

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Gun: 플레이어 캐릭터를 찾을 수 없습니다!"));
        return;
    }

    FName WeaponSocket = "GunSocket_R";
    AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);

    // 자동으로 Bullet Factory 설정
    AutoAssignBulletFactory();

    UE_LOG(LogTemp, Warning, TEXT("%s가 플레이어 손에 장착됨!"), *GetName());
}

void AGun::SetGunMeshAndBullet(UStaticMesh* NewMesh, TSubclassOf<ABullet> NewBulletFactory)
{
    if (NewMesh)
    {
        GunStaticMesh->SetStaticMesh(NewMesh);
        UE_LOG(LogTemp, Warning, TEXT("총 메쉬 변경 완료: %s"), *NewMesh->GetName());
    }

    if (NewBulletFactory)
    {
        BulletFactory = NewBulletFactory;
        UE_LOG(LogTemp, Warning, TEXT("총알 공장 설정 완료"));
    }
}

/** 🔥 BP_Bullet을 자동으로 설정하는 함수 */
void AGun::AutoAssignBulletFactory()
{
    if (BulletFactory)
    {
        return; // 이미 설정되어 있다면 변경하지 않음
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet Factory 자동 할당 실패: World가 없습니다!"));
        return;
    }

    // 기본 Bullet Blueprint 경로
    FString BulletPath = TEXT("/Game/Items/Blueprints/BP_Bullet.BP_Bullet_C");

    // 블루프린트에서 탄환 클래스 로드
    UClass* BulletClass = LoadClass<ABullet>(nullptr, *BulletPath);
    if (BulletClass)
    {
        BulletFactory = BulletClass;
        UE_LOG(LogTemp, Warning, TEXT("Bullet Factory 자동 설정 완료! BP_Bullet이 설정되었습니다."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet Factory 자동 설정 실패! 블루프린트 경로를 확인하세요."));
    }
}
