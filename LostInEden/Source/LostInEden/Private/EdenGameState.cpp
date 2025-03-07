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
	SetStageIndex(EEdenStageIndex::Aisle1);
}

void AEdenGameState::EndLevel()
{
	// 게임클리어 => n초 뒤 TitleLevel로 이동
	
	

}

void AEdenGameState::RestartLevel()
{
	// 게임오버 => 버튼 누르면 Main레벨 다시 시작



}

void AEdenGameState::OnGameClear()
{
	// 게임클리어 => 클리어 UI 띄우기

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && GameClearWidgetClass)
	{
		UUserWidget* GameClearWidget = CreateWidget<UUserWidget>(PC, GameClearWidgetClass);
		if (GameClearWidget)
		{
			GameClearWidget->AddToViewport();
			//PC->SetShowMouseCursor(true);
			//PC->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AEdenGameState::OnGameOver()
{
	// 게임오버 => 게임오버 UI 띄우기
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && GameOverWidgetClass)
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(PC, GameOverWidgetClass);
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport();
			//PC->SetShowMouseCursor(true);
			//PC->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AEdenGameState::UpdateHUD()
{

	// Stage 관련 출력
}

void AEdenGameState::SetStageIndex(EEdenStageIndex _Index)
{
	CurStageIndex = _Index;
}

void AEdenGameState::SettingStage(EEdenStageIndex _Index)
{

}