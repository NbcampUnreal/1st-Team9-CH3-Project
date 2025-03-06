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
	// 게임 클리어 => n초 뒤 TitleLevel로 이동
	
	

}

void AEdenGameState::RestartLevel()
{
	// 게임오버 => Title 레벨로 이동



}

void AEdenGameState::OnGameClear()
{
	// 게임 클리어 = > 클리어 UI 띄우기



}

void AEdenGameState::OnGameOver()
{
	// 게임오버 => 게임오버스크린 띄우기
	


}

void AEdenGameState::UpdateHUD()
{

	// Stage 관련 출력
}

void AEdenGameState::SetStageIndex(EEdenStageIndex _Index)
{
	CurStageIndex = _Index;
}

void AEdenGameState::SettingStage(EEdenStageIndex _Index)
{

}