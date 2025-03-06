// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EdenGameInstance.generated.h"

UCLASS()
class LOSTINEDEN_API UEdenGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEdenGameInstance();

	void Init() override;

	// Getter, Setter
	UFUNCTION(BlueprintCallable, Category = "Level")
	FName GetCurLevelName();
	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetTotalScore();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void SetCurLevelName(FName _Name);
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetTotalScore(int32 _Score);

	// Update Data
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void UpdateData(FName _Name, int32 _Score);

protected:
	// Level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelNames;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	FName CurLevelName;

	// Score
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 TotalScore;
};
