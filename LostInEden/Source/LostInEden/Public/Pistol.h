#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Pistol.generated.h"

UCLASS()
class LOSTINEDEN_API APistol : public AGun
{
    GENERATED_BODY()

public:
    APistol();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsAutomatic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread;


    virtual void Fire() override;
    virtual void Reload() override;
};
