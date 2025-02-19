// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINEDEN_API APlayerCharacter : public AEntity
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

	void Heal();
	void ChangeState();
	
	void TakeDamaged(int32);
};
