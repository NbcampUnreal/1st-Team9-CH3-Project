// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameState.h"

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
	SetStage(0);
}

void ATitleGameState::EndLevel()
{
	// �������� Ŭ���� => Ending ����(���� ����)�� �̵�
	// ���ӿ��� => ���ӿ�����ũ�� ���� Title ������ �̵�

	// �������� => 
}

void ATitleGameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));

	// Title ������ �̵�
}

void ATitleGameState::SetStage(int32 _Index)
{
	CurStageIndex = static_cast<ETitleStageIndex>(_Index);
}