#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Engine/DataTable.h"
#include "BulletData.h"
#include "PlayerCharacter.h"
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

    UPROPERTY(EditAnywhere, Category = "Effects")
    class USoundBase* bulletSound;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* MuzzleFlash;  

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* ImpactEffect;



    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UDataTable* BulletDataTable;


    virtual void BeginPlay() override;
    virtual void Fire() override;
    virtual void Reload() override;


};
