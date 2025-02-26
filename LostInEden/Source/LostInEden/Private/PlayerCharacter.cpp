// Copyright Epic Games, Inc. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gun.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Shotgun.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	NormalSpeed = 600.f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	// 앉기 동작을 수행할 수 있게 해주는 bool값
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	MaxHealth = 200;
	Health = MaxHealth;

	MaxShieldGauge = 50;
	ShieldGauge = 0;

	//기본 권총 장착
	EquippedWeapon = EGunType::PISTOL;
}

int32 APlayerCharacter::GetShieldGauge() const
{
	return ShieldGauge;
}

int32 APlayerCharacter::GetMaxShieldGauge() const
{
	return MaxShieldGauge;
}

AGun* APlayerCharacter::GetCurrentWeapon() const
{
	if (EquipInventory.Contains(EquippedWeapon))
	{
		return *EquipInventory.Find(EquippedWeapon);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 장착된 총기 없음!!"));
		return nullptr;
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipGun(EGunType::PISTOL);
}

void APlayerCharacter::Heal(int32 HealAmount)
{
	Health = FMath::Clamp<int32>(Health + HealAmount, 0, MaxHealth);
}

void APlayerCharacter::ChangeState(EPlayerStatus State)
{
	UE_LOG(LogTemp, Warning, TEXT("Change State!!"));
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

void APlayerCharacter::StartAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Gun Fire!!"));
	AGun* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void APlayerCharacter::StopAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Fire!!"));
	AGun* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		if (ARifle* Rifle = Cast<ARifle>(CurrentWeapon))
		{
			Rifle->StopAutoFire();
		}
	}
}

void APlayerCharacter::ReloadAmmo()
{
	//아이템 탄약 확인해서 장전하기 구현해야함

	AGun* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void APlayerCharacter::UseItem(AItem* CurrItem)
{
	UE_LOG(LogTemp, Warning, TEXT("Use Item!!"));
}

void APlayerCharacter::EquipGun(EGunType GunType)
{
	//임시
	UClass* GunClass;
	/*if (GetWorld())
	{
		EquippedWeapon = GetWorld()->SpawnActor<AGun>(Pistol);
	}*/

	switch (GunType)
	{
	case EGunType::PISTOL:
		if (EquipInventory.Contains(EGunType::PISTOL))
		{
			EquippedWeapon = *EquipInventory.Find(EGunType::PISTOL);
		}

		if (!EquippedWeapon)
		{
			GunClass = LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Pistol.BP_Pistol_C"));
			if(GetWorld())
			{
				EquippedWeapon = GetWorld()->SpawnActor<AGun>(GunClass);
				EquipInventory.Add({ EGunType::PISTOL, EquippedWeapon });
			}
		}
		break;
	case EGunType::RIFLE:
		if (EquipInventory.Contains(EGunType::RIFLE))
		{
			EquippedWeapon = *EquipInventory.Find(EGunType::RIFLE);
		}

		if (!EquippedWeapon)
		{
			GunClass = LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Rifle.BP_Rifle_C"));
			if (GetWorld())
			{
				EquippedWeapon = GetWorld()->SpawnActor<AGun>(GunClass);
				EquipInventory.Add({ EGunType::RIFLE, EquippedWeapon });
			}
		}
		break;
	case EGunType::SHOTGUN:
		if (EquipInventory.Contains(EGunType::SHOTGUN))
		{
			EquippedWeapon = *EquipInventory.Find(EGunType::SHOTGUN);
		}

		if (!EquippedWeapon)
		{
			GunClass = LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Shotgun.BP_Shotgun_C"));
			if (GetWorld())
			{
				EquippedWeapon = GetWorld()->SpawnActor<AGun>(GunClass);
				EquipInventory.Add({ EGunType::SHOTGUN, EquippedWeapon });
			}
		}
		break;
	default:
		break;
	}

	if (EquippedWeapon)
	{
		FName GunSocketName = "GunSocket_R";
		if (GetMesh()->DoesSocketExist(GunSocketName))
		{
			EquippedWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GunSocketName);
		}
		EquippedWeapon->SetOwner(this);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopSprint
				);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::DoCrouch
				);
			}

			if (PlayerController->AttackAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AttackAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartAttack
				);

				EnhancedInput->BindAction(
					PlayerController->AttackAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopAttack
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::ReloadAmmo
				);
			}

			if (PlayerController->ChangeGunAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ChangeGunAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::SelectGun
				);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	if (!Controller) return;

	if (Value.Get<bool>())
	{
		Jump();
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Controller) return;

	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::StartSprint(const FInputActionValue& Value)
{
	if (!Controller) return;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& Value)
{
	if (!Controller) return;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void APlayerCharacter::DoCrouch(const FInputActionValue& Value)
{
	CanCrouch() ? Crouch() : UnCrouch();
}

void APlayerCharacter::SelectGun(const FInputActionValue& Value)
{
	
}

