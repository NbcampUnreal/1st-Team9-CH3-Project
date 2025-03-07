// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameState.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


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
	// ����Ŭ���� => ���� Ŭ���� 10�� �� ȣ��, Title������ �̵�
	
	FName NextLevelName = FName(TEXT("MenuLevel"));
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_TitleGameMode";

	LevelName = NextLevelName;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}

void AEdenGameState::RestartLevel()
{
	// ���ӿ��� => ����� ��ư ������ ȣ��, Main���� �ٽ� ����

	FName NextLevelName = FName(TEXT("Demo_Map"));
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_EdenGameMode";

	LevelName = NextLevelName;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}

void AEdenGameState::OnGameClear()
{
	// ����Ŭ���� => ���� ��� �� ȣ��, Ŭ���� UI ����

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && GameClearWidgetClass)
	{
		UUserWidget* GameClearWidget = CreateWidget<UUserWidget>(PC, GameClearWidgetClass);
		if (GameClearWidget)
		{
			GameClearWidget->AddToViewport();
			PC->SetShowMouseCursor(true);
			//PC->SetInputMode(FInputModeUIOnly());
		}
	}

	GetWorldTimerManager().SetTimer(EndLevelTimerHandle, this, &AEdenGameState::EndLevel, 10.0f, false);
}

void AEdenGameState::OnGameOver()
{
	// ���ӿ��� => �÷��̾� ��� �� ȣ��, ���ӿ��� UI ����
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && GameOverWidgetClass)
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(PC, GameOverWidgetClass);
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport();
			PC->SetShowMouseCursor(true);
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