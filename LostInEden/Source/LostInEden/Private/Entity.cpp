// Copyright Epic Games, Inc. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"

// Sets default values
AEntity::AEntity()
{
	MaxHealth = 100;
	Health = MaxHealth;
}

int32 AEntity::GetHealth()
{
	return Health;
}

int32 AEntity::GetMaxHealth()
{
	return MaxHealth;
}

float AEntity::TakeDamage(float AmountDamage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(AmountDamage, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp<int32>(Health - Damage, 0, MaxHealth);

	return Damage;
}