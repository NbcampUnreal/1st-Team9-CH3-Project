// Copyright Epic Games, Inc. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"

// Sets default values
AEntity::AEntity()
{
	MaxHealth = 100;
	Health = MaxHealth;
}

float AEntity::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp<int32>(Health - Damage, 0, MaxHealth);

	return 0.0f;
}