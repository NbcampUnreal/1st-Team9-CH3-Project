#include "Gun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"

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
		UE_LOG(LogTemp, Warning, TEXT("총 발사! 남은 탄약: %d"), CurrentAmmo);
		CurrentAmmo--;

		if (BulletFactory)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
				FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
				World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, MuzzleRot);
			}
		}

		if (MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, MuzzleLocation->GetComponentLocation());
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
