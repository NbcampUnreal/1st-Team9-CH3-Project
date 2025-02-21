#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Rifle.generated.h"

UCLASS()
class LOSTINEDEN_API ARifle : public AGun
{
	GENERATED_BODY() // ✅ 반드시 포함해야 함

public:
	ARifle();

	virtual void Fire() override;
	virtual void Reload() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bIsAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 BurstCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BurstFireRate;

private:
	FTimerHandle AutoFireHandle;
	void StartAutoFire();
	void StopAutoFire();
	void BurstFire();
};
