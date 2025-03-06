// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h" // ĳ���� ��� ����
#include "PlayerHUDWidget.h" // HUD ���� ��� ����
#include "Gun.h" // ���� ��� ����

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

	// ĳ���� ���� �ʱ�ȭ
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	// HUD ���� ����
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	// �ʱ� HUD ������Ʈ
	UpdateHUD();
}


void AMainPlayerController::UpdateHUD()
{
	if (UPlayerHUDWidget* Widget = Cast<UPlayerHUDWidget>(HUDWidget))
	{
		// ĳ���� ����
		APlayerCharacter* MyPlayerCharacter = Cast<APlayerCharacter>(GetPawn());
		if (!PlayerCharacter) return;

		// ü�� �� �������� (AEntity�� Getter ���)
		int32 CurrentHealth = PlayerCharacter->GetHealth();
		int32 MaxHealth = PlayerCharacter->GetMaxHealth();

		// ���� �� �������� (APlayerCharacter�� Getter ���)
		int32 CurrentShield = PlayerCharacter->GetShieldGauge();
		int32 MaxShield = PlayerCharacter->GetMaxShieldGauge();

		// ȸ�� ���� ���� ��������
		int32 PotionCount = MyPlayerCharacter->GetHealPotionCnt();

		// HUD ������Ʈ
		Widget->UpdateHealth(CurrentHealth, MaxHealth);
		Widget->UpdateShield(CurrentShield, MaxShield);
		Widget->UpdatePotionCount(PotionCount);

		// ���� ������ ���� ��������
		AGun* CurrentWeapon = MyPlayerCharacter->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			int32 CurrentAmmo = CurrentWeapon->GetCurrentAmmo();
			int32 MaxAmmo = CurrentWeapon->GetMaxAmmo();

			// ź�� ������ UI�� ������Ʈ 
			Widget->UpdateAmmo(CurrentAmmo, MaxAmmo);
		}
		else
		{
			// ���Ⱑ ���� ��� ź�� 0���� ����
			Widget->UpdateAmmo(0, 0);
		}
	}
}