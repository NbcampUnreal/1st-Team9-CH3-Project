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
                FVector ShotDirection = MuzzleRot.Vector(); // 🔹 MuzzleLocation 기준 발사 방향

                // 🔹 총구에서 라인트레이스 시작
                FVector TraceStart = MuzzlePos;
                FVector TraceEnd = TraceStart + (ShotDirection * 10000.0f);

                FHitResult HitResult;
                FCollisionQueryParams QueryParams;
                QueryParams.AddIgnoredActor(this);

                bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
                DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 1.0f);

                if (bHit)
                {
                    ShotDirection = (HitResult.ImpactPoint - MuzzlePos).GetSafeNormal();

                    // 🔹 Apply Damage 추가
                    float DamageAmount = 10.0f; // 피해량 설정
                    AActor* HitActor = HitResult.GetActor();
                    if (HitActor)
                    {
                        UGameplayStatics::ApplyDamage(
                            HitActor,
                            DamageAmount,
                            GetOwner()->GetInstigatorController(), // 피해를 준 주체 (플레이어)
                            this,
                            nullptr
                        );

                        UE_LOG(LogTemp, Warning, TEXT("총알이 %s에 명중! 피해량: %f"), *HitActor->GetName(), DamageAmount);
                    }
                }

                // 🔹 총알 생성
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
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("총알 스폰 실패!"));
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