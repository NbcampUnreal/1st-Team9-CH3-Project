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

	StartLevel();
}

void ATitleGameState::StartLevel()
{
	// Initializing

	UpdateStateData();
	SetStageIndex(0);
}

void ATitleGameState::EndLevel()
{
	// �ó׸�ƽ ���� �� ȣ��, MainLevel�� �Ѿ��

	FName NextLevelName = FName("Demo_Map");
	int32 NextLevelIndex = GameInstance->GetLevelIndexByName(NextLevelName);
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_TitleGameMode";

	LevelIndex = NextLevelIndex;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}

void ATitleGameState::SetStageIndex(int32 _Index)
{
	CurStageIndex = static_cast<ETitleStageIndex>(_Index);
}