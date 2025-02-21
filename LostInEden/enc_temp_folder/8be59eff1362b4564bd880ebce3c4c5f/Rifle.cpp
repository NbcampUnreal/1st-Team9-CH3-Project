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
	if (bIsAutomatic)
	{
		StartAutoFire();
	}
	else
	{
		BurstFire();
	}
}

void ARifle::StartAutoFire()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
		UE_LOG(LogTemp, Warning, TEXT("자동사격 남은 탄약: %d"), CurrentAmmo);
		if (CurrentAmmo > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ARifle::StartAutoFire, FireRate, false);
		}
	}
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
