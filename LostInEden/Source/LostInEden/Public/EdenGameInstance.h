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
	int32 GetLevelIndex();
	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetTotalScore();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void SetLevelIndex(int32 _Index);
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetTotalScore(int32 _Score);

	// Update Data
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void UpdateData(int32 _LevelIndex, int32 _Score);

protected:
	// Level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelNames;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurLevelIndex;

	// Score
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 TotalScore;
};
