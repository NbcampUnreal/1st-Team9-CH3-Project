// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GlobalEnum.h"
#include "GunManager.generated.h"

class AGun;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOSTINEDEN_API UGunManager : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	TMap<EGunType, AGun*> WeaponPool;

	TSet<EGunType> OwnedWeapons;

public:
	UGunManager();

	void InitWeaponPool(UWorld* World);

	TArray<EGunType> GetOwnedGunList() const;

	AGun* GetWeapon(EGunType GunType) const;

	void AcquireWeapon(EGunType GunType);

	bool HasWeapon(EGunType GunType) const;
};
