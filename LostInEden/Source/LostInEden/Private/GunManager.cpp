// Fill out your copyright notice in the Description page of Project Settings.


#include "GunManager.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Shotgun.h"

UGunManager::UGunManager()
{
	APistol* Pistol = CreateDefaultSubobject<APistol>(TEXT("Pistol"));
	OwnedGuns.Add({ EGunType::PISTOL, Pistol->GetCurrentAmmo() });

	//�ӽ�
	ARifle* Rifle = CreateDefaultSubobject<ARifle>(TEXT("Rifle"));
	AShotgun* Shotgun = CreateDefaultSubobject<AShotgun>(TEXT("Shotgun"));
	OwnedGuns.Add({ EGunType::RIFLE, Rifle->GetCurrentAmmo() });
	OwnedGuns.Add({ EGunType::SHOTGUN, Shotgun->GetCurrentAmmo() });
}

TArray<EGunType> UGunManager::GetOwnedGunList()
{
	TArray<EGunType> KeyArray;
	OwnedGuns.GenerateKeyArray(KeyArray);
	return KeyArray;
}

void UGunManager::UpdateGunData(TPair<EGunType, AGun*> CurrGun)
{
	OwnedGuns.Add({ CurrGun.Key, CurrGun.Value->GetCurrentAmmo() });
}

void UGunManager::SetCurrentGun(TPair<EGunType, AGun*> NextGun)
{
	if (OwnedGuns.Find(NextGun.Key))
	{
		NextGun.Value->SetCurrentAmmo(NextGun.Value->GetCurrentAmmo());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("�ش� ���� �κ��丮�� �����ϴ�!"));
	}
}
