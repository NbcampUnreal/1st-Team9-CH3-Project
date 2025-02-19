// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "PlayerCharacter.generated.h"

enum class EPlayerStatus
{
	
};

UCLASS()
class LOSTINEDEN_API APlayerCharacter : public AEntity
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

	void Heal(int32);
	void ChangeState(EPlayerStatus);
	
	void TakeDamaged(int32);
	virtual void UseItem(class AItem*) override;
};
