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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	EEdenStageIndex CurrentStage;

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

private:
	void StartLevel();
	void EndLevel();
	void UpdateHUD();
};
