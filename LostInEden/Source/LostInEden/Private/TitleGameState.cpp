// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameState.h"
#include "Kismet/GameplayStatics.h"
#include "EdenGameInstance.h"

ATitleGameState::ATitleGameState()
{

}

void ATitleGameState::BeginPlay()
{
	Super::BeginPlay();

}

void ATitleGameState::UpdateStateData()
{
	// Update GameData from GameInstance to GameState

	UEdenGameInstance* GameInstance = Cast<UEdenGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		LevelIndex = GameInstance->GetLevelIndex();
		Score = GameInstance->GetTotalScore();
	}
}

void ATitleGameState::UpdateInstanceData()
{
	// Update GameData from GameState to GameInstance

	UEdenGameInstance* GameInstance = Cast<UEdenGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		GameInstance->UpdateData(LevelIndex, Score);
	}
}