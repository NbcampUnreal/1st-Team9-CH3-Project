// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameInstance.h"

UEdenGameInstance::UEdenGameInstance()
{

}

void UEdenGameInstance::Init()
{
	// Initializing

	LevelNames = { FName(TEXT("MenuLevel")), FName(TEXT("Demo_Map")) };
	UpdateData(FName(TEXT("MenuLevel")), 0);
}

void UEdenGameInstance::UpdateData(FName _Name, int32 _Score)
{
	SetCurLevelName(_Name);
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