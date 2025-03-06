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

	// Updating Methods
	UFUNCTION(BlueprintCallable, Category = "Level")
	void UpdateLevelIndex(int32 _Index);
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddScore(int32 _Score);

protected:
	// Level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelNames;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 LevelIndex;
	
	// Score
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 TotalScore;
};
