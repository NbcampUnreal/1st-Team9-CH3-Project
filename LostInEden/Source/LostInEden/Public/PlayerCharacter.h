// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

enum class EPlayerStatus
{

};

enum EGunType : int8
{
	PISTOL,
	RIFLE,
	SHOTGUN
};

enum EItemType : int8
{
	SHIELD,
	HEALINGITEM
};

UCLASS()
class LOSTINEDEN_API APlayerCharacter : public AEntity
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	int32 GetShieldGauge() const;
	int32 GetMaxShieldGauge() const;

	AGun* GetCurrentWeapon() const;

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
	TMap<EGunType, class AGun* > EquipInventory;
	TMap<EItemType, class AItem*> ItemInventory;

	//현재 장착된 총
	EGunType EquippedWeapon;

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
	void SelectGun(const FInputActionValue& Value);

	virtual void BeginPlay() override;

public:
	void Heal(int32);
	void ChangeState(EPlayerStatus);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void StartAttack();
	void StopAttack();
	void ReloadAmmo();
	void UseItem(class AItem*);
	void EquipGun(EGunType);
};