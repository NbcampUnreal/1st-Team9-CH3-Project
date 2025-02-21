#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Shotgun.generated.h"

UCLASS()
class LOSTINEDEN_API AShotgun : public AGun
{
	GENERATED_BODY()

public:
	AShotgun();

	virtual void Fire() override;
	virtual void Reload() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 PelletCount;  // 샷건 한 발에 발사될 탄환 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float PelletSpread;  // 탄환 퍼짐 정도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadTime;  // 재장전 시간
};
