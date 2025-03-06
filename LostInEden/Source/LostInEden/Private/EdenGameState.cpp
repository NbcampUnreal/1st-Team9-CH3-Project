// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameState.h"


AEdenGameState::AEdenGameState()
{

}

void AEdenGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
}

void AEdenGameState::StartLevel()
{
	// Initializing

	UpdateStateData();
	SetStage(0);
}

void AEdenGameState::EndLevel()
{
	/*
	정상적인 클리어 => Ending 레벨(다음 레벨)로 이동
	게임오버 => 게임오버스크린 띄우고 Title 레벨로 이동
	
	강제종료 => 
	*/

}

void AEdenGameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));

	// Title 레벨로 이동
}

void AEdenGameState::UpdateHUD()
{

	// Stage 관련 출력
}

void AEdenGameState::SetStage(int32 _Index)
{
	CurStageIndex = static_cast<EEdenStageIndex>(_Index);
}