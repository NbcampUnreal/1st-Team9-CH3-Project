// Copyright Epic Games, Inc. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"

// Sets default values
AEntity::AEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = false;

	MaxHealth = 100;
	Health = MaxHealth;
}

void AEntity::TakeDamaged(int32 Damage)
{
	Health -= Damage;
	if (Health < 0)
	{
		Health = 0;
	}
}
