// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EdenGameState.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINEDEN_API AEdenGameState : public AGameState
{
	GENERATED_BODY()

public:
	AEdenGameState();

	virtual void BeginPlay() override;

	void StartLevel();
	void EndLevel();
	void UpdateHUD();

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

private:

};
