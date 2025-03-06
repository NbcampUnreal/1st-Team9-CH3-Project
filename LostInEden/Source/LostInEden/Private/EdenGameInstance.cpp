// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameInstance.h"

UEdenGameInstance::UEdenGameInstance()
{

}

void UEdenGameInstance::Init()
{
	CurLevelIndex = 0;
	TotalScore = 0;

	// TODO : 레벨 목록 초기화
}

void UEdenGameInstance::UpdateData(int32 _Index, int32 _Score)
{
	SetLevelIndex(_Index);
	SetTotalScore(_Score);
}

int32 UEdenGameInstance::GetLevelIndex()
{
	return CurLevelIndex;
}

int32 UEdenGameInstance::GetTotalScore()
{
	return TotalScore;
}

void UEdenGameInstance::SetLevelIndex(int32 _Index)
{
	CurLevelIndex = _Index;
}

void UEdenGameInstance::SetTotalScore(int32 _Score)
{
	TotalScore = _Score;
}