#pragma once

#include "CoreMinimal.h"
#include "Gun.h"  
#include "PlayerCharacter.h"
#include "Shotgun.generated.h"

UCLASS()
class AShotgun : public AGun  
{
    GENERATED_BODY()

public:
    AShotgun();

protected:
    virtual void BeginPlay() override;

    int32 PelletCount;   
    float PelletSpread;  
public:
    void Fire() override;  
    void Reload() override; 
    void FinishReload();
    void ResetFire(); 
protected:
 

    UPROPERTY(EditAnywhere, Category = "Weapon Stats")
    float ReloadTime; 

    UPROPERTY(EditAnywhere, Category = "Weapon Stats")
    bool bCanFire; 

    FTimerHandle FireDelayTimer; 
    FTimerHandle ReloadTimer; 

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TEnumAsByte<EGunType> GunType = EGunType::SHOTGUN;

    UPROPERTY(EditAnywhere, Category = "Effects")
    class USoundBase* bulletSound;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* MuzzleFlash;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* ImpactEffect;


};