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

}

void AParentGameState::UpdateStateData()
{
	// Update GameData from GameInstance to GameState

	UEdenGameInstance* GameInstance = Cast<UEdenGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		LevelIndex = GameInstance->GetLevelIndex();
		Score = GameInstance->GetTotalScore();
	}
}

void AParentGameState::UpdateInstanceData()
{
	// Update GameData from GameState to GameInstance

	UEdenGameInstance* GameInstance = Cast<UEdenGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		GameInstance->UpdateData(LevelIndex, Score);
	}
}

void AParentGameState::StartLevel()
{

}

void AParentGameState::EndLevel()
{

}

void AParentGameState::OnGameOver()
{

}