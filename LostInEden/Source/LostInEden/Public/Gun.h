#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Bullet.h"
#include "IFireable.h"
#include "Gun.generated.h"

UCLASS()
class LOSTINEDEN_API AGun : public AItem, public IIFireable
{
	GENERATED_BODY() // ✅ 반드시 포함해야 함

public:
	AGun();  // ✅ 기본 생성자 선언

	virtual void Fire() override;
	virtual void Reload() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

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
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* MuzzleLocation;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ABullet> BulletFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UParticleSystem* ImpactEffect;

};
