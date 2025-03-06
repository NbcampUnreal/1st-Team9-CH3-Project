// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameState.h"
#include "Kismet/GameplayStatics.h"
#include "EdenGameInstance.h"


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
	//Init

	UpdateStateData();
	CurStageIndex = EEdenStageIndex::Aisle1;
}

void AEdenGameState::EndLevel()
{
	// 정상적인 클리어 => Ending 레벨(다음 레벨)로 이동
	// 게임오버 => 게임오버스크린 띄우고 Title 레벨로 이동

	// 강제종료 => 
}

void AEdenGameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));

	// Title 레벨로 이동
}

void AEdenGameState::UpdateStateData()
{
	// Update GameData from GameInstance to GameState

	UEdenGameInstance* GameInstance = Cast<UEdenGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		LevelIndex = GameInstance->GetLevelIndex();
		Score = GameInstance->GetTotalScore();
	}
}

void AEdenGameState::UpdateInstanceData()
{
	// Update GameData from GameState to GameInstance

	UEdenGameInstance* GameInstance = Cast<UEdenGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		GameInstance->UpdateData(LevelIndex, Score);
	}
}

void AEdenGameState::UpdateHUD()
{

	// Stage 관련 출력
}