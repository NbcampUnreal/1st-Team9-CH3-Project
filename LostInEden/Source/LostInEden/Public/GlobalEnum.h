// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalEnum.generated.h"

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


UCLASS()
class LOSTINEDEN_API UGlobalEnum : public UObject
{
	GENERATED_BODY()
	
};
