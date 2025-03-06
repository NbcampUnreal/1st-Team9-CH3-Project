// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
//#include "GlobalEnum.generated.h"


// 게임 전반에서 Global하게 사용되는 Enum Class 모음


/// <summary>
/// Title Level에서 사용되는 Stage 정보
/// </summary>
UENUM(BlueprintType)
enum class ETitleStageIndex : uint8
{
	Title		UMETA(DisplayName = "타이틀"),
	Scenematic	UMETA(DisplayName = "시네마틱영상")
};

/// <summary>
/// Main Level에서 사용되는 Stage 정보
/// </summary>
UENUM(BlueprintType)
enum class EEdenStageIndex : uint8
{
	Aisle1		UMETA(DisplayName = "통로1"),
	Wave1		UMETA(DisplayName = "웨이브1"),
	Aisle2		UMETA(DisplayName = "통로2"),
	Wave2		UMETA(DisplayName = "웨이브2"),
	Aisle3		UMETA(DisplayName = "통로3"),
	Boss		UMETA(DisplayName = "보스"),
};

/// <summary>
/// Ending Level에서 사용되는 Stage 정보
/// </summary>
UENUM(BlueprintType)
enum class EEndingStageIndex : uint8
{
	Ending		UMETA(DisplayName = "엔딩"),
};


/*
UCLASS()
class LOSTINEDEN_API UGlobalEnum : public UObject
{
	GENERATED_BODY()
	
};
*/