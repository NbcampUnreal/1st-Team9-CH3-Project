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
	// ����Ŭ���� => n�� �� TitleLevel�� �̵�
	
	

}

void AEdenGameState::RestartLevel()
{
	// ���ӿ��� => ��ư ������ Main���� �ٽ� ����



}

void AEdenGameState::OnGameClear()
{
	// ����Ŭ���� => Ŭ���� UI ����

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
	// ���ӿ��� => ���ӿ��� UI ����
	
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

	// Stage ���� ���
}

void AEdenGameState::SetStageIndex(EEdenStageIndex _Index)
{
	CurStageIndex = _Index;
}

void AEdenGameState::SettingStage(EEdenStageIndex _Index)
{

}