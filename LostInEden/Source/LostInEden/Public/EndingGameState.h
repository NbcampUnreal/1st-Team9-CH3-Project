// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentGameState.h"
#include "GlobalEnum.h"
#include "EndingGameState.generated.h"

// Ending ·¹º§ÀÇ GameState

UCLASS()
class LOSTINEDEN_API AEndingGameState : public AParentGameState
{
	GENERATED_BODY()
	
public:
	AEndingGameState();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	EEndingStageIndex CurrentStage;
};
