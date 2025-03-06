// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentGameState.h"
#include "TitleGameState.generated.h"

// Title ·¹º§ÀÇ GameState

UCLASS()
class LOSTINEDEN_API ATitleGameState : public AParentGameState
{
	GENERATED_BODY()

public:
	ATitleGameState();

	virtual void BeginPlay() override;

protected:
	// Level, Stage Information
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	ETitleStageIndex CurStageIndex;

};
