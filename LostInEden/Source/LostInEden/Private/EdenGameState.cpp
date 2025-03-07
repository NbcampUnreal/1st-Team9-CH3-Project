// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenGameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "SpawnVolume.h"
#include "EnemyCharacter.h"


AEdenGameState::AEdenGameState()
{

}

void AEdenGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
}

void AEdenGameState::StartLevel()
{
	// Initializing

	UpdateStateData();
	SetStageIndex(EEdenStageIndex::Aisle1);
	SetPlayerState(EPlayerState::Playing);

	SettingStage(EEdenStageIndex::Wave1);
}

void AEdenGameState::EndLevel()
{
	// ����Ŭ���� => ���� Ŭ���� 10�� �� ȣ��, Title������ �̵�
	
	FName NextLevelName = FName(TEXT("MenuLevel"));
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_TitleGameMode";

	LevelName = NextLevelName;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}

void AEdenGameState::RestartLevel()
{
	// ���ӿ��� => ����� ��ư ������ ȣ��, Main���� �ٽ� ����

	FName NextLevelName = FName(TEXT("Demo_Map"));
	FString NextGameMode = "GameMode=/Game/Blueprints/BP_EdenGameMode";

	LevelName = NextLevelName;
	UpdateInstanceData();

	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, NextGameMode);
}

void AEdenGameState::OnGameClear()
{
	// ����Ŭ���� => ���� ��� �� ȣ��, Ŭ���� UI ����

	if (PlayerState == EPlayerState::GameOver)
	{
		return;
	}

	// PlayerState
	SetPlayerState(EPlayerState::GameClear);

	// UI
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && GameClearWidgetClass)
	{
		UUserWidget* GameClearWidget = CreateWidget<UUserWidget>(PC, GameClearWidgetClass);
		if (GameClearWidget)
		{
			GameClearWidget->AddToViewport();
			PC->SetShowMouseCursor(true);
			//PC->SetInputMode(FInputModeUIOnly());
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}

	// Open Level
	GetWorldTimerManager().SetTimer(EndLevelTimerHandle, this, &AEdenGameState::EndLevel, 10.0f, false);
}

void AEdenGameState::OnGameOver()
{
	// ���ӿ��� => �÷��̾� ��� �� ȣ��, ���ӿ��� UI ����

	if (PlayerState == EPlayerState::GameClear)
	{
		return;
	}

	// PlayerState
	SetPlayerState(EPlayerState::GameOver);

	// UI
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && GameOverWidgetClass)
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(PC, GameOverWidgetClass);
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport();
			PC->SetShowMouseCursor(true);
			//PC->SetInputMode(FInputModeUIOnly());
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

void AEdenGameState::UpdateHUD()
{

	// Stage ���� ���
}

void AEdenGameState::SetStageIndex(EEdenStageIndex _Index)
{
	CurStageIndex = _Index;
}

void AEdenGameState::SetPlayerState(EPlayerState _State)
{
	PlayerState = _State;
}

void AEdenGameState::SettingStage(EEdenStageIndex _Index)
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 NumberToSpawn = 4;

	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			//UClass* LoadedClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/AI/BP_EnemyCharacter_Ranged.BP_EnemyCharacter_Ranged_C"));

			if (SpawnVolume && SpawningClass)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnActorFromSpawnVolume(SpawningClass);

				if (SpawnedActor && SpawnedActor->IsA(AEnemyCharacter::StaticClass()))
				{
					int a = 0;
				}
			}
		}
	}
}