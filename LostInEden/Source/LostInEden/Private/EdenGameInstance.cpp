// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameInstance.h"

UEdenGameInstance::UEdenGameInstance()
{

}

void UEdenGameInstance::Init()
{
	LevelNames = { FName("MenuLevel"), FName("Demo_Map") };
	//CurLevelIndex = 0;
	TotalScore = 0;

	// TODO : ���� ��� �ʱ�ȭ
}

void UEdenGameInstance::UpdateData(FName _Name, int32 _Score)
{
	//SetLevelIndex(_Index);
	SetTotalScore(_Score);
}

FName UEdenGameInstance::GetCurLevelName()
{
	return CurLevelName;
}

int32 UEdenGameInstance::GetTotalScore()
{
	return TotalScore;
}

void UEdenGameInstance::SetCurLevelName(FName _Name)
{
	CurLevelName = _Name;
}

void UEdenGameInstance::SetTotalScore(int32 _Score)
{
	TotalScore = _Score;
}