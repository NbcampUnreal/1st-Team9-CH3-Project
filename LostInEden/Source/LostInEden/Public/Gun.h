#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Bullet.h"
#include "IFireable.h"
#include "Gun.generated.h"

UCLASS()
class LOSTINEDEN_API AGun : public AItem, public IIFireable
{
    GENERATED_BODY()

public:
    AGun();

<<<<<<< HEAD
=======
    int32 GetCurrentAmmo() const;
    int32 GetMaxAmmo() const;

>>>>>>> parent of 0478c4b (Merge branch 'dev.player' into Item)
    virtual void Fire() override;
    virtual void Reload() override;

protected:
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
};
