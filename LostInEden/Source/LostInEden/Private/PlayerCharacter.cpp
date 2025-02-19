// Copyright Epic Games, Inc. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	Health = 200;
}

void APlayerCharacter::Heal()
{
}

void APlayerCharacter::ChangeState()
{
}

void APlayerCharacter::TakeDamaged(int32 Damage)
{
	Super::TakeDamaged(Damage);
}
