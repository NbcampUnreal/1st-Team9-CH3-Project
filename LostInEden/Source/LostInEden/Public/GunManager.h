// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalEnum.h"
#include "UObject/NoExportTypes.h"
#include "GunManager.generated.h"

class AGun;

UCLASS()
class LOSTINEDEN_API UGunManager : public UObject
{
	GENERATED_BODY()

protected:
	TMap<EGunType, int32> OwnedGuns;

public:
	UGunManager();

	TArray<EGunType> GetOwnedGunList();

	void UpdateGunData(TPair<EGunType, AGun*>);
	void SetCurrentGun(TPair<EGunType, AGun*>);
};
