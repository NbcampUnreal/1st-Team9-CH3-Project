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
	SetStageIndex(EEdenStageIndex::Aisle1);
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

void AEdenGameState::SetStageIndex(EEdenStageIndex _Index)
{
	CurStageIndex = _Index;
}

void AEdenGameState::SettingStage(EEdenStageIndex _Index)
{

}