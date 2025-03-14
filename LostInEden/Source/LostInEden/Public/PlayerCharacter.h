// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalEnum.h"
#include "Entity.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class AGun;
class AItem;
class UGunManager;
class AHealingItem;
class AShield;
class USoundBase;

UCLASS()
class LOSTINEDEN_API APlayerCharacter : public AEntity
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	void SetHealth(int32);
	void SetShieldGauge(int32);

	int32 GetShieldGauge() const;
	int32 GetMaxShieldGauge() const;
	int32 GetHealPotionCnt() const;
	TMap<EItemType, int32>& GetAmmoInventory();

	// 현재 장착하고 있는 무기의 객체를 반환
	AGun* GetCurrentWeapon();

	UFUNCTION()
	void OnOverlapBegin(
		class UPrimitiveComponent* OverlappedComponent,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnOverlapEnd(
		class UPrimitiveComponent* OverlappedComponent,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

protected:
	// 쉴드 게이지
	int32 ShieldGauge;
	int32 MaxShieldGauge;

	AShield* Shield;

	// 이동 속도 관련
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;

	// 플레이어 인벤토리
	TMap<EItemType, int32> AmmoInventory;
	//힐링 아이템
	AHealingItem* HealPotion;

	// Gun 매니저
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UGunManager* GunManager;

	// 현재 장착된 무기 타입과 객체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AGun* CurrWeapon;

	// 주울수 있는 아이템 리스트
	TArray<AItem*> OverlappingItemList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsDead;

	bool bCanChangeGun;

	FTimerHandle InputDelayTimerHandle;
	FTimerHandle DelayDieState;

	USoundBase* ZoomInSound;
	USoundBase* ZoomOutSound;

	// 카메라 관련 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|Camera")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|Camera")
	class UCameraComponent* Camera;

	// 박스 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|Collision")
	class UBoxComponent* BoxCollisionComp;

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
	UFUNCTION()
	void PickupItem(const FInputActionValue& Value);

	virtual void BeginPlay() override;

public:
	// 캐릭터 상태 변화
	void ChangeState(EPlayerStatus);

	// 데미지를 처리하는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	// 무기 공격
	void StartAttack();
	void StopAttack();

	// 총 재장전
	void ReloadAmmo();

	// 줌 인/아웃 사운드 재생
	void PlayZoomInSound();
	void PlayZoomOutSound();

	// 아이템 사용
	void UseItem();
	// 아이템 추가
	void AddItem(AItem*);

	// 무기 장착
	void EquipWeapon(EGunType);

	// 무기 스왑시 타이머 세팅값 바꾸기
	void ResetInput();

	// 플레이어가 죽었을 때
	void Die();

	// 플레이어의 상태가 변화했을 때 UI 업데이트
	void UpdateUI();
};