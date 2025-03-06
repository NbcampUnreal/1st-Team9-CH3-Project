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
	// 시네마틱 종료 후 호출, MainLevel로 넘어가기

	FName NextLevelName = FName(TEXT("Demo_Map"));
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_EdenGameMode";

	LevelName = NextLevelName;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}
