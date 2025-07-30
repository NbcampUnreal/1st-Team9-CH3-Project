// Fill out your copyright notice in the Description page of Project Settings.


#include "GunManager.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Shotgun.h"

UGunManager::UGunManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	OwnedGuns.Add(EGunType::PISTOL, 500);
    OwnedGuns.Add({ EGunType::RIFLE, 0 });
    OwnedGuns.Add({ EGunType::SHOTGUN, 0 });
}

TArray<EGunType> UGunManager::GetOwnedGunList() const
{
    TArray<EGunType> KeyArray;
    OwnedGuns.GenerateKeyArray(KeyArray);
    return KeyArray;
}


void UGunManager::UpdateGunData(AGun* CurrGun)
{
	OwnedGuns.Add({ CurrGun->GetGunType(), CurrGun->GetCurrentAmmo() });
}

void UGunManager::SetCurrentGun(AGun* NextGun)
{
	if (int32* Ammo = OwnedGuns.Find(NextGun->GetGunType()))
	{
		NextGun->SetCurrentAmmo(*Ammo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("해당 무기 인벤토리에 존재하지 않음!"));
	}
}

UGunManager::~UGunManager()
{
	UE_LOG(LogTemp, Error, TEXT("건매니저 사라짐!!!!!"));
}

