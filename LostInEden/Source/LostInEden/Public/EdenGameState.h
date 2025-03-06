// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void SetStageIndex(int32 _Index) override;

protected:
	// Level, Stage Information
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	EEdenStageIndex CurStageIndex;

	void StartLevel() override;
	void EndLevel() override;
	void RestartLevel() override;
	void OnGameClear() override;
	void OnGameOver() override;

	// Stage : Boss
	FTimerHandle BossTimerHandle;

	// HUD
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHUD();
};
