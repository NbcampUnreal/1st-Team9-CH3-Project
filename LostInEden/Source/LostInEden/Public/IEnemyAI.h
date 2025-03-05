// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEnemyAI.generated.h"

UENUM(BlueprintType)
enum class E_MovementSpeed : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Sprinting UMETA(DisplayName = "Sprinting")
};

UINTERFACE(MinimalAPI)
class UIEnemyAI : public UInterface
{
	GENERATED_BODY()
};

class LOSTINEDEN_API IIEnemyAI
{
	GENERATED_BODY()

public:
	virtual void Heal(float HealPercentage) = 0;
	virtual void Attack() = 0;
	virtual void EquipWeapon() = 0;
	virtual void UnequipWeapon() = 0;
	virtual void GetIdealRange(float& AttackRadius, float& DefendRadius) = 0;
	virtual void SetMovementSpeed(E_MovementSpeed Speed, float& SpeedValue) = 0;
	virtual class AActor* GetPatrolRoute() const = 0;

};
