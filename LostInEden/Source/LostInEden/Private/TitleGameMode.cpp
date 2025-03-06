// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameState.h"

ATitleGameMode::ATitleGameMode()
{
    // GameMode ���� ��� ���� ����
    {
        static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBP(TEXT("/Game/Young/BP_MenuGamecontroller"));
        if (PlayerControllerBP.Succeeded())
        {
            PlayerControllerClass = PlayerControllerBP.Class;
        }

        static ConstructorHelpers::FClassFinder<AGameState> GameStateBP(TEXT("/Game/Blueprints/BP_TitleGameState"));
        if (GameStateBP.Succeeded())
        {
            GameStateClass = GameStateBP.Class;
        }
    }
}