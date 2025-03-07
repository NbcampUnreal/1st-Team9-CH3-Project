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
	// 게임클리어 => 게임 클리어 10초 뒤 호출, Title레벨로 이동
	
	FName NextLevelName = FName(TEXT("MenuLevel"));
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_TitleGameMode";

	LevelName = NextLevelName;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}

void AEdenGameState::RestartLevel()
{
	// 게임오버 => 재시작 버튼 누르면 호출, Main레벨 다시 시작

	FName NextLevelName = FName(TEXT("Demo_Map"));
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_EdenGameMode";

	LevelName = NextLevelName;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}

void AEdenGameState::OnGameClear()
{
	// 게임클리어 => 보스 사망 시 호출, 클리어 UI 띄우기

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
	// 게임오버 => 플레이어 사망 시 호출, 게임오버 UI 띄우기
	
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

	// Stage 관련 출력
}

void AEdenGameState::SetStageIndex(EEdenStageIndex _Index)
{
	CurStageIndex = _Index;
}

void AEdenGameState::SettingStage(EEdenStageIndex _Index)
{

}