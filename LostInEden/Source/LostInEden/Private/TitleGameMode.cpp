// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameState.h"

ATitleGameMode::ATitleGameMode()
{
    // GameMode 구성 요소 직접 지정 -> TODO : 타이틀레벨에서만 사용되는 컨트롤러와 스테이트로 변경 필요
    /*{
        static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBP(TEXT("/Game/Blueprints/BP_MainPlayerController"));
        if (PlayerControllerBP.Succeeded())
        {
            PlayerControllerClass = PlayerControllerBP.Class;
        }

        static ConstructorHelpers::FClassFinder<AGameState> GameStateBP(TEXT("/Game/Blueprints/BP_TitleGameState"));
        if (GameStateBP.Succeeded())
        {
            GameStateClass = GameStateBP.Class;
        }
    }*/
}