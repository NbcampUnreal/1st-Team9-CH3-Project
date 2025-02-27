#pragma once

#include "CoreMinimal.h"
#include "Gun.h"  // ✅ AGun을 상속받기 위해 추가
#include "Shotgun.generated.h"

UCLASS()
class AShotgun : public AGun  // ✅ AActor → AGun으로 변경
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
private:
    void AutoAssignBulletFactory();
};