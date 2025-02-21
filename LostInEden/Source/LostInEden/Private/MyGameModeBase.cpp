// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "PlayerCharacter.h"
#include "MainPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
}
