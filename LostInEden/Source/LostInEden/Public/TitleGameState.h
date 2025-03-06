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

	// Update Methods
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void UpdateStateData();
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void UpdateInstanceData();

protected:
	// Level, Stage Information
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 LevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	ETitleStageIndex CurStageIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 Score;

};
