#pragma once  // ✅ 중복 포함 방지

#include "CoreMinimal.h"
#include "Gun.h"
#include "Rifle.generated.h"

UCLASS()
class LOSTINEDEN_API ARifle : public AGun
{
    GENERATED_BODY()

public:
    ARifle();

    virtual void Fire() override;
    virtual void Reload() override;

private:
    // ✅ EquippedGun 변수가 nullptr일 가능성이 있으므로 사용하기 전에 반드시 확인 필요!
    AActor* EquippedGun;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsAutomatic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 BurstCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BurstFireRate;

public:
    FTimerHandle AutoFireHandle;
    void StartAutoFire();
    void StopAutoFire();
    void BurstFire();
};
