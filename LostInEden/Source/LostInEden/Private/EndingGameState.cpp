// Fill out your copyright notice in the Description page of Project Settings.


#include "EndingGameState.h"

AEndingGameState::AEndingGameState()
{

}

void AEndingGameState::BeginPlay()
{
	Super::BeginPlay();

}

void AEndingGameState::SetStage(int32 _Index)
{
	CurStageIndex = static_cast<EEndingStageIndex>(_Index);
}