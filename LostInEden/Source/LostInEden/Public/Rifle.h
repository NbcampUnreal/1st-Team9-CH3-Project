#pragma once  // ✅ 중복 포함 방지

#include "CoreMinimal.h"
#include "Gun.h"
#include "PlayerCharacter.h"
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
    
    AActor* EquippedGun;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsAutomatic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 BurstCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BurstFireRate;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TEnumAsByte<EGunType> GunType = EGunType::RIFLE;


public:
    FTimerHandle AutoFireHandle;
    void StartAutoFire();
    void StopAutoFire();
    void BurstFire();

};
