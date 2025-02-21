#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "TestCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGun::AGun() // ✅ 기본 생성자 정의 추가
{
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	SetRootComponent(GunMesh);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GunMesh);

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

				// 🔹 총알 생성
				ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, MuzzleRot);
				if (SpawnedBullet)
				{
					// 🔹 총알이 앞 방향으로 나아가도록 힘을 추가
					UProjectileMovementComponent* BulletMovement = SpawnedBullet->FindComponentByClass<UProjectileMovementComponent>();
					if (BulletMovement)
					{
						BulletMovement->Velocity = MuzzleRot.Vector() * BulletMovement->InitialSpeed;
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

	// 🔹 플레이어 캐릭터 가져오기
	ATestCharacter* PlayerCharacter = Cast<ATestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Gun: 플레이어 캐릭터를 찾을 수 없습니다!"));
		return;
	}

	// 🔹 플레이어 손에 장착
	FName WeaponSocket = "GunSocket_R";  // 🔹 블루프린트에서 설정한 손 소켓 이름
	AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);

	UE_LOG(LogTemp, Warning, TEXT("%s가 플레이어 손에 장착됨!"), *GetName());
}

