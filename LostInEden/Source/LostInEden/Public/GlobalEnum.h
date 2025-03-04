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
};

/// <summary>
/// Main Level에서 사용되는 Stage 정보
/// </summary>
UENUM(BlueprintType)
enum class EEdenStageIndex : uint8
{
	Ground		UMETA(DisplayName = "지상통로"),
	Basement1	UMETA(DisplayName = "지하통로1"),
	Wave1		UMETA(DisplayName = "웨이브1"),
	Basement2	UMETA(DisplayName = "지하통로2"),
	Wave2		UMETA(DisplayName = "웨이브2"),
	Basement3	UMETA(DisplayName = "지하통로3"),
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
e.g.
UENUM(BlueprintType)
enum class EGlobalEnum : uint8
{
	Stage1	UMETA(DisplayName = "지상"),
	Stage2	UMETA(DisplayName = "지하"),
};
*/

/*
UCLASS()
class LOSTINEDEN_API UGlobalEnum : public UObject
{
	GENERATED_BODY()
	
};
*/