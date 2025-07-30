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

	mutable TArray<EGunType> CachedGunList;
	mutable bool bGunListCacheDirty = true;

public:
	UGunManager();

	const TArray<EGunType>& GetOwnedGunList() const;

	void UpdateGunData(const AGun&);
	void SetCurrentGun(AGun&);

	bool HasGun(EGunType) const;
	int32 GetGunCnt() const;

private:
	void UpdateGunListCache() const;

	~UGunManager();
};
