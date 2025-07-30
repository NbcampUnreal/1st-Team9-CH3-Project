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
	TMap<EGunType, int32> OwnedGuns;

public:
	UGunManager();

	TArray<EGunType> GetOwnedGunList() const;

	void UpdateGunData(AGun*);
	void SetCurrentGun(AGun*);

	~UGunManager();
};
