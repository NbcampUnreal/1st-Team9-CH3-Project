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
}

void ATitleGameState::EndLevel()
{
	// �ó׸�ƽ ���� �� ȣ��, Main ������ �̵�

	FName NextLevelName = FName(TEXT("Demo_Map"));
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_EdenGameMode";

	LevelName = NextLevelName;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}
