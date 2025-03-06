// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GlobalEnum.h"
#include "ParentGameState.generated.h"

// Custom GameState 들의 부모

UCLASS()
class LOSTINEDEN_API AParentGameState : public AGameState
{
	GENERATED_BODY()

public:
	AParentGameState();

	virtual void BeginPlay() override;

	// Getter, Setter
	UFUNCTION(BlueprintCallable, Category = "GameData")
	virtual void SetStageIndex(int32 _Index);

	// Update Methods
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void UpdateStateData();
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void UpdateInstanceData();

	// Setting Methods
	UFUNCTION(BlueprintCallable, Category = "GameData")
	virtual void SettingStage(int32 _Index);

protected:
	// Level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	FName LevelName;

	UFUNCTION(BlueprintCallable, Category = "Level")
	virtual void StartLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	virtual void EndLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	virtual void RestartLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	virtual void OnGameClear();
	UFUNCTION(BlueprintCallable, Category = "Level")
	virtual void OnGameOver();

	// Game Data
	class UEdenGameInstance* GameInstance = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 Score;
};
