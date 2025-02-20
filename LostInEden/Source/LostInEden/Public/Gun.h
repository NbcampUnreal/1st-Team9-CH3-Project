// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "IFireable.h"
#include "Gun.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINEDEN_API AGun : public AItem, public IIFireable
{
	GENERATED_BODY()
	

public:
    AGun();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
    int32 MaxAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
    int32 CurrentAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
    int32 MagazineSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
    float Range;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* MuzzleEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* ImpactEffect;

    virtual void Fire() override;
    virtual void Reload() override;
};
