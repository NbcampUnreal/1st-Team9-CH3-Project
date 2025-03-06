// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GlobalEnum.h"
#include "TitleGameState.generated.h"

// Title ·¹º§ÀÇ GameState

UCLASS()
class LOSTINEDEN_API ATitleGameState : public AGameState
{
	GENERATED_BODY()

public:
	ATitleGameState();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	ETitleStageIndex CurrentStage;
};
