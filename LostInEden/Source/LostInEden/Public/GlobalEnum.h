// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Global�ϰ� ���Ǵ� enum class

/* 
e.g.
UENUM(BlueprintType)
enum class EStageIndex : uint8
{
	Stage1	UMETA(DisplayName = "����"),
	Stage2	UMETA(DisplayName = "����"),
};
*/

enum class EPlayerStatus
{

};

UENUM(BlueprintType)
enum EGunType : int8
{
	PISTOL		UMETA(DisplayName = "Pistol"),
	RIFLE		UMETA(DisplayName = "Rifle"),
	SHOTGUN		UMETA(DisplayName = "Shotgun"),
	GUN			UMETA(DisplayName = "DefaultGun")
};

UENUM(BlueprintType)
enum EItemType : int8
{
	SHIELD		UMETA(DisplayName = "Shield"),
	HEALINGITEM	UMETA(DisplayName = "HealingItem"),
	NONE		UMETA(DisplayName = "None")
};