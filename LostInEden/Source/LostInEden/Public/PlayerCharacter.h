// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

enum class EPlayerStatus
{

};

UENUM(BlueprintType)
enum EGunType : int8
{
	PISTOL		UMETA(DisplayName = "Pistol"),
	RIFLE		UMETA(DisplayName = "Rifle"),
	SHOTGUN		UMETA(DisplayName = "Shotgun")
};

UENUM(BlueprintType)
enum EItemType : int8
{
	SHIELD		UMETA(DisplayName = "Shield"),
	HEALINGITEM	UMETA(DisplayName = "HealingItem")
};

UCLASS()
class LOSTINEDEN_API APlayerCharacter : public AEntity
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	int32 GetShieldGauge() const;
	int32 GetMaxShieldGauge() const;

	class AGun* GetCurrentWeapon();

protected:
	// 카메라 관련 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	// 이동 속도 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;

	// 플레이어 인벤토리
	TMap<EGunType, class AGun*> EquipInventory;
	TMap<EItemType, class AItem*> ItemInventory;

	//현재 장착된 무기 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TEnumAsByte<EGunType> CurrentWeapon;
	//현재 장착된 무기 블루프린트 객체
	AGun* BP_Weapon;

	//쉴드
	int32 ShieldGauge;
	int32 MaxShieldGauge;

	// IMC
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);
	UFUNCTION()
	void DoCrouch(const FInputActionValue& Value);
	UFUNCTION()
	void SelectWeapon(const FInputActionValue& Value);

	virtual void BeginPlay() override;

public:
	void Heal(int32);
	void ChangeState(EPlayerStatus);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void StartAttack();
	void StopAttack();
	void ReloadAmmo();
	void UseItem(class AItem*);
	void EquipWeapon(EGunType);
	void AddWeapon(EGunType);
};