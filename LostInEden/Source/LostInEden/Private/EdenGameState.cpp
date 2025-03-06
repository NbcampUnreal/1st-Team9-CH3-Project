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
	SetStageIndex(0);
}

void AEdenGameState::EndLevel()
{
	// ���� Ŭ���� => n�� �� TitleLevel�� �̵�
	
	

}

void AEdenGameState::RestartLevel()
{
	// ���ӿ��� => Title ������ �̵�



}

void AEdenGameState::OnGameClear()
{
	// ���� Ŭ���� = > Ŭ���� UI ����



}

void AEdenGameState::OnGameOver()
{
	// ���ӿ��� => ���ӿ�����ũ�� ����
	


}

void AEdenGameState::UpdateHUD()
{

	// Stage ���� ���
}

void AEdenGameState::SetStageIndex(int32 _Index)
{
	CurStageIndex = static_cast<EEdenStageIndex>(_Index);
}