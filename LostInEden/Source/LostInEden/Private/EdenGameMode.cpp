// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameState.h"

AEdenGameMode::AEdenGameMode()
{
    // GameMode 备己 夸家 流立 瘤沥
    {
        static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBP(TEXT("/Game/Character/Player/BP_PlayerCharacter"));
        if (PlayerPawnBP.Succeeded())
        {
            DefaultPawnClass = PlayerPawnBP.Class;
        }

        static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBP(TEXT("/Game/Blueprints/BP_MainPlayerController"));
        if (PlayerControllerBP.Succeeded())
        {
            PlayerControllerClass = PlayerControllerBP.Class;
        }

        static ConstructorHelpers::FClassFinder<AGameState> GameStateBP(TEXT("/Game/Blueprints/BP_EdenGameState"));
        if (GameStateBP.Succeeded())
        {
            GameStateClass = GameStateBP.Class;
        }
    }
}

void AEdenGameMode::BeginPlay()
{

}

void AEdenGameMode::Tick(float _DeltaTime)
{

}