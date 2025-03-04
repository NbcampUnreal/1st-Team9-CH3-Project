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

	// 초기화 요소들
}

void AEdenGameState::EndLevel()
{

	// Ending 레벨로 이동
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