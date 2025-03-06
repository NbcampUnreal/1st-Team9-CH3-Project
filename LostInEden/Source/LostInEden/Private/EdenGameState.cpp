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
	// �������� Ŭ���� => Ending ����(���� ����)�� �̵�
	// ���ӿ��� => ���ӿ�����ũ�� ���� Title ������ �̵�

	// �������� => 
}

void AEdenGameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));

	// Title ������ �̵�
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

	// Stage ���� ���
}