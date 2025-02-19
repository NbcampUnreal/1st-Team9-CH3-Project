// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"

UCLASS(Abstract)
class LOSTINEDEN_API AEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntity();

protected:
	int32 Health;
	int32 MaxHealth;
	//Gun* Weapon;

public:
	void TakeDamaged(int32);
	virtual void UseItem() PURE_VIRTUAL(AEntity::UseItem, ;);
};
