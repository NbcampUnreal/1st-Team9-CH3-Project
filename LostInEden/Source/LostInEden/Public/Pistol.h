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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsAutomatic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread;

    class USoundBase* bulletSound;
 

    virtual void BeginPlay() override;
    virtual void Fire() override;
    virtual void Reload() override;

private:
    void AutoAssignBulletFactory();

};
