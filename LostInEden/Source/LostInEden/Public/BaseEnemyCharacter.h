// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.h"
#include "IEnemyAI.h"
#include "Animation/AnimMontage.h"
#include "Materials/MaterialInstance.h"
#include "BaseEnemyCharacter.generated.h"

class UWidgetComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAnimMontage;

UCLASS()
class LOSTINEDEN_API ABaseEnemyCharacter : public AEntity, public IIEnemyAI
{
	GENERATED_BODY()

public:
	ABaseEnemyCharacter();

	//----�������̽� ��� �޼���----//
	UFUNCTION(BlueprintCallable)
	virtual void Heal(float HealPercentage) override;
	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;
	virtual void EquipWeapon() override;
	virtual void UnequipWeapon() override;
	UFUNCTION(BlueprintCallable)
	virtual void GetIdealRange(float& AttackRadius, float& DefendRadius) override;
	UFUNCTION(BlueprintCallable)
	virtual void SetMovementSpeed(E_MovementSpeed Speed, float& SpeedValue) override;
	UFUNCTION(BlueprintCallable)
	virtual class AActor* GetPatrolRoute() const override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties")
	bool IsWieldingWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties")
	bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties")
	AActor* PatrolRoute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties|UI")
	UWidgetComponent* HealthBarWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties|Appearance")
	AActor* WeaponActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties|Appearance")
	UAnimMontage* HitReactionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties|Appearance")
	UMaterialInstanceDynamic* MaterialInstance0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties|Appearance")
	UMaterialInstanceDynamic* MaterialInstance1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Properties|Appearance")
	UNiagaraSystem* NiagaraSystem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Properties|Appearance")
	UNiagaraComponent* NiagaraComponent;
	FTimerHandle PlayDeadAnimTimerHandle;
	

	bool bIsLerping;	// ���� �÷���
	float CurrentAlpha;	
	float Lerp_A;
	float Lerp_B;
	float LerpDuration;	// ���� �ð�
	float ElapsedTime;	// ��� �ð�

public:	
	
	UFUNCTION(BlueprintCallable)
	float TakeDamage(float AmountDamage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	// ���� �÷��̽� ü�¹ٴ� �Ⱥ��� ����
	//UFUNCTION(BlueprintCallable)
	//void UpdateHealth();
	// �� ����� ȣ��
	UFUNCTION(BlueprintCallable)
	void OnDead();
	UFUNCTION(BlueprintCallable)
	void PlayDeadAnim();
	// �ǰݽ� ȣ��
	UFUNCTION(BlueprintCallable)
	void OnStunned();
	UFUNCTION(BlueprintCallable, Category = "Lerp")
	void StartLerp(float InStartValue, float InEndValue, float InDuration);
	void UpdateLerpedValues(float LerpedValue);
};
