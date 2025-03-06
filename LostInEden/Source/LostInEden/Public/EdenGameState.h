// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GlobalEnum.h"
#include "EdenGameState.generated.h"

// Main ·¹º§ÀÇ GameState

UCLASS()
class LOSTINEDEN_API AEdenGameState : public AGameState
{
	GENERATED_BODY()

public:
	AEdenGameState();

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
	EEdenStageIndex CurStageIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 Score;

	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void EndLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	// Stage : Boss
	FTimerHandle BossTimerHandle;

	// HUD
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHUD();
};
