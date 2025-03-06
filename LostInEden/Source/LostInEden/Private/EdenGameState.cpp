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
	// Initializing

	UpdateStateData();
	SetStage(0);
}

void AEdenGameState::EndLevel()
{
	/*
	�������� Ŭ���� => Ending ����(���� ����)�� �̵�
	���ӿ��� => ���ӿ�����ũ�� ���� Title ������ �̵�
	
	�������� => 
	*/

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

void AEdenGameState::SetStage(int32 _Index)
{
	CurStageIndex = static_cast<EEdenStageIndex>(_Index);
}