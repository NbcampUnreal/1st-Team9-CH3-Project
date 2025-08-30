// Fill out your copyright notice in the Description page of Project Settings.


#include "GunManager.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Shotgun.h"

UGunManager::UGunManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	OwnedWeapons.Add(EGunType::PISTOL);

	//테스트용
	OwnedWeapons.Add(EGunType::RIFLE);
	OwnedWeapons.Add(EGunType::SHOTGUN);
}

void UGunManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World가 nullptr입니다!"));
		return;
	}

	TMap<EGunType, UClass*> WeaponClasses = {
		{EGunType::PISTOL, LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Pistol.BP_Pistol_C"))},
		{EGunType::RIFLE, LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Rifle.BP_Rifle_C"))},
		{EGunType::SHOTGUN, LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Shotgun.BP_Shotgun_C"))}
	};

	for (auto& WeaponPair : WeaponClasses)
	{
		EGunType GunType = WeaponPair.Key;
		UClass* GunClass = WeaponPair.Value;

		if (GunClass)
		{
			AGun* WeaponInstance = World->SpawnActor<AGun>(GunClass);
			if (WeaponInstance)
			{
				WeaponInstance->SetActorHiddenInGame(true);
				WeaponInstance->SetActorEnableCollision(false);

				WeaponPool.Add(GunType, WeaponInstance);
				UE_LOG(LogTemp, Warning, TEXT("무기 풀에 추가됨: %d"), GunType);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("무기 클래스 로드 실패: %d"), GunType);
		}
	}
}

TArray<EGunType> UGunManager::GetOwnedGunList() const
{
	return OwnedWeapons.Array();
}

AGun* UGunManager::GetWeapon(EGunType GunType) const
{
	if (AGun* const* FoundWeapon = WeaponPool.Find(GunType))
	{
		return *FoundWeapon;
	}

	UE_LOG(LogTemp, Warning, TEXT("무기 풀에서 해당 무기를 찾을 수 없음: %d"), GunType);
	return nullptr;
}

void UGunManager::AcquireWeapon(EGunType GunType)
{
	if (!HasWeapon(GunType))
	{
		OwnedWeapons.Add(GunType);
		UE_LOG(LogTemp, Warning, TEXT("새로운 무기 획득: %d"), GunType);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("이미 소지한 무기입니다: %d"), GunType);
	}
}

bool UGunManager::HasWeapon(EGunType GunType) const
{
	return OwnedWeapons.Contains(GunType);
}
