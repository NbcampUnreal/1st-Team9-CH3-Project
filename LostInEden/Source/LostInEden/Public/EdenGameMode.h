// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EdenGameMode.generated.h"

// Main ������ GameMode

UCLASS()
class LOSTINEDEN_API AEdenGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AEdenGameMode();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

};
