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
            UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음!"));
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

//NewGun->SetGunMeshAndBullet(NewGunMesh, NewBulletFactory);