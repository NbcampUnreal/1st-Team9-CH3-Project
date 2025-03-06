#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Bullet.h"
#include "IFireable.h"
#include "GlobalEnum.h"
#include "Gun.generated.h"

UCLASS()
class LOSTINEDEN_API AGun : public AItem, public IIFireable
{
    GENERATED_BODY()

public:
    AGun();


    int32 GetCurrentAmmo() const;
    int32 GetMaxAmmo() const;
    EGunType GetGunType() const;
    EItemType GetAmmoType() const;

    int32 GetAmmoFromInventory(int32 Amount);
    void SetCurrentAmmo(int32 NewAmmo);

    UFUNCTION(BlueprintCallable)
    virtual void Fire() override;
    virtual void Reload() override;


protected:
    // 🔹 무기 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 MaxAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 CurrentAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Range;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<class ABullet> BulletFactory;

    // 🔹 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* GunStaticMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* MuzzleLocation;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* SceneRoot;  // ✅ 루트 컴포넌트

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TEnumAsByte<EGunType> GunType;
};
