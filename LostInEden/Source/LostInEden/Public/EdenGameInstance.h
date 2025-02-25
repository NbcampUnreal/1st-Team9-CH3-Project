// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EdenGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EStageIndex : uint8
{
	Stage1 = 0,
	Stage2,
};

UCLASS()
class LOSTINEDEN_API UEdenGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UEdenGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Data")
	EStageIndex CurrentStage;
};
