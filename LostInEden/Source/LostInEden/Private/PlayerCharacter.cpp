// Copyright Epic Games, Inc. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	MaxHealth = 200;
	Health = MaxHealth;
}

void APlayerCharacter::Heal(int32 HealAmount)
{
	Health += HealAmount;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void APlayerCharacter::ChangeState(EPlayerStatus State)
{
}

void APlayerCharacter::TakeDamaged(int32 Damage)
{
	Super::TakeDamaged(Damage);
}

void APlayerCharacter::UseItem(AItem* CurrItem)
{

}
