// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentGameState.h"
#include "TitleGameState.generated.h"

// Title ������ GameState

UCLASS()
class LOSTINEDEN_API ATitleGameState : public AParentGameState
{
	GENERATED_BODY()

public:
	ATitleGameState();

	virtual void BeginPlay() override;

	// Getter, Setter
	void SetStageIndex(int32 _Index) override;

protected:
	// Stage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	ETitleStageIndex CurStageIndex;

	void StartLevel() override;
	void EndLevel() override;

};
