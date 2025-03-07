// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ParentGameState.h"
#include "GlobalEnum.h"
#include "EdenGameState.generated.h"

// Main ·¹º§ÀÇ GameState

UCLASS()
class LOSTINEDEN_API AEdenGameState : public AParentGameState
{
	GENERATED_BODY()

public:
	AEdenGameState();

	virtual void BeginPlay() override;

	// Getter, Setter
	void SetStageIndex(EEdenStageIndex _Index);
	void SetPlayerState(EPlayerState _State);

	// Level
	void OnGameClear() override;
	void OnGameOver() override;

protected:
	// Level
	void StartLevel() override;
	void EndLevel() override;
	void RestartLevel() override;

	// Stage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	EEdenStageIndex CurStageIndex;

	UFUNCTION(BlueprintCallable, Category = "Stage")
	void SettingStage(EEdenStageIndex _Index);

	// Stage : Boss
	FTimerHandle BossTimerHandle;
	FTimerHandle EndLevelTimerHandle;

	// HUD
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameClearWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHUD();

	// PlayerState
	EPlayerState PlayerState;
};
