// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Global하게 사용되는 enum class

/* 
e.g.
UENUM(BlueprintType)
enum class EStageIndex : uint8
{
	Stage1	UMETA(DisplayName = "지상"),
	Stage2	UMETA(DisplayName = "지하"),
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
	SHOTGUN		UMETA(DisplayName = "Shotgun")
};

UENUM(BlueprintType)
enum EItemType : int8
{
	SHIELD		UMETA(DisplayName = "Shield"),
	HEALINGITEM	UMETA(DisplayName = "HealingItem"),
	NONE		UMETA(DisplayName = "None")
};