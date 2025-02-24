// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

enum class EPlayerStatus
{
	
};

UCLASS()
class LOSTINEDEN_API APlayerCharacter : public AEntity
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	// ī�޶� ���� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	// �̵� �ӵ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;

	// �÷��̾� �κ��丮
	TArray<class AGun*> EquipInventory;
	TArray<class AItem*> ItemInventory;

	// �κ��丮 ���� �� �ӽ� ����
	//TUniquePtr<class AGun> EquippedWeapon;
	AGun* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AGun> GunClass;

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
	
	virtual void BeginPlay() override;

public:
	void Heal(int32);
	void ChangeState(EPlayerStatus);
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void StartAttack();
	void StopAttack();
	void ReloadAmmo();
	void UseItem(class AItem*);
	void EquipGun();
};