// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameState.h"

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

	// �ʱ�ȭ ��ҵ�
}

void AEdenGameState::EndLevel()
{

	// Ending ������ �̵�
}

void AEdenGameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));

	// Title ������ �̵�
}

void AEdenGameState::UpdateHUD()
{

	// Stage ���� ���
}