#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Bullet.h"
#include "PlayerCharacter.h"
#include "IFireable.h"
#include "Gun.generated.h"

UCLASS()
class LOSTINEDEN_API AGun : public AItem, public IIFireable
{
    GENERATED_BODY()

public:
    AGun();

    int32 GetCurrentAmmo() const;
    int32 GetMaxAmmo() const;

    virtual void Fire() override;
    virtual void Reload() override;
    virtual void BeginPlay() override;

    /** BP_Bullet을 자동으로 할당하는 함수 */
    void AutoAssignBulletFactory();

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

public:

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* GunStaticMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* MuzzleLocation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<class ABullet> BulletFactory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* MuzzleEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* ImpactEffect;

    void SetGunMeshAndBullet(UStaticMesh* NewMesh, TSubclassOf<ABullet> NewBulletFactory);
};
