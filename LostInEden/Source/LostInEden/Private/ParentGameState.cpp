// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentGameState.h"
#include "Kismet/GameplayStatics.h"
#include "EdenGameInstance.h"

AParentGameState::AParentGameState()
{

}

void AParentGameState::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UEdenGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AParentGameState::UpdateStateData()
{
	// Update GameData from GameInstance to GameState

	if (GameInstance)
	{
		LevelName = GameInstance->GetCurLevelName();
		Score = GameInstance->GetTotalScore();
	}
}

void AParentGameState::UpdateInstanceData()
{
	// Update GameData from GameState to GameInstance

	if (GameInstance)
	{
		GameInstance->UpdateData(LevelName, Score);
	}
}

void AParentGameState::StartLevel()
{

}

void AParentGameState::EndLevel()
{

}

void AParentGameState::RestartLevel()
{

}

void AParentGameState::OnGameClear()
{

}

void AParentGameState::OnGameOver()
{

}

void AParentGameState::SetStageIndex(int32 _Index)
{

}

void AParentGameState::SettingStage(int32 _Index)
{

}