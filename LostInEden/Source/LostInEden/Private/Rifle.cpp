#include "Rifle.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ARifle::ARifle()
{
	Damage = 15.0f;
	FireRate = 0.1f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	Range = 3000.0f;

	bIsAutomatic = true;
	BulletSpread = 2.0f;
	BurstCount = 3;
	BurstFireRate = 0.15f;
}

void ARifle::Fire()
{
	if (CurrentAmmo > 0)
	{
		StartAutoFire(); // 🔹 마우스 버튼을 누르면 자동 사격 시작
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
	}
}

void ARifle::StartAutoFire()
{
	if (CurrentAmmo > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("자동사격 남은 탄약: %d"), CurrentAmmo);
		CurrentAmmo--;

		// 총알 발사
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

		// 자동 사격 반복 실행 (마우스 버튼을 누르고 있을 때만)
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ARifle::StartAutoFire, FireRate, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("탄약 없음!"));
	}
}

void ARifle::StopAutoFire()
{
	// 🔹 마우스 버튼을 떼면 자동 사격 중단
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
	UE_LOG(LogTemp, Warning, TEXT("소총 재장전!"));
	CurrentAmmo = MaxAmmo;
}
