// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h" // 캐릭터 헤더 포함
#include "PlayerHUDWidget.h" // HUD 위젯 헤더 포함

AMainPlayerController::AMainPlayerController():
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	CrouchAction(nullptr),
	AttackAction(nullptr),
	ReloadAction(nullptr),
	ChangeGunAction(nullptr),
	PickupAction(nullptr),
	UseItemAction(nullptr)
{
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	// 캐릭터 참조 초기화
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	// HUD 위젯 생성
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	// 초기 HUD 업데이트
	UpdateHUD();
}


void AMainPlayerController::UpdateHUD()
{
	if (UPlayerHUDWidget* Widget = Cast<UPlayerHUDWidget>(HUDWidget))
	{
		// 캐릭터 참조
		APlayerCharacter* MyPlayerCharacter = Cast<APlayerCharacter>(GetPawn());
		if (!PlayerCharacter) return;

		// 체력 값 가져오기 (AEntity의 Getter 사용)
		int32 CurrentHealth = PlayerCharacter->GetHealth();
		int32 MaxHealth = PlayerCharacter->GetMaxHealth();

		// 쉴드 값 가져오기 (APlayerCharacter의 Getter 사용)
		int32 CurrentShield = PlayerCharacter->GetShieldGauge();
		int32 MaxShield = PlayerCharacter->GetMaxShieldGauge();

		// HUD 업데이트
		Widget->UpdateHealth(CurrentHealth, MaxHealth);
		Widget->UpdateShield(CurrentShield, MaxShield);
	}
}