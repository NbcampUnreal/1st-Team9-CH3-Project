// Copyright Epic Games, Inc. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GunManager.h"
#include "Gun.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Shotgun.h"
#include "Shield.h"
#include "HealingItem.h"
#include "Magazine.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	// 드롭된 아이템을 인식하기 위한 충돌 박스
	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComp->SetupAttachment(RootComponent);
	BoxCollisionComp->SetBoxExtent(FVector(30.0f, 30.f, 30.f));
	BoxCollisionComp->AddLocalOffset(FVector(50.f, 0.f, -55.f));

	// 오버랩 이벤트 바인딩
	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

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
	
	// 건 매니저 생성
	GunManager = CreateDefaultSubobject<UGunManager>(TEXT("GunManager"));
	CurrWeapon = nullptr;
}

void APlayerCharacter::SetHealth(int32 HealthAmount)
{
	Health = HealthAmount;
}

void APlayerCharacter::SetShieldGauge(int32 ShieldAmount)
{
	ShieldGauge = ShieldAmount;
}

int32 APlayerCharacter::GetShieldGauge() const
{
	return ShieldGauge;
}

int32 APlayerCharacter::GetMaxShieldGauge() const
{
	return MaxShieldGauge;
}

TMap<EItemType, int32>& APlayerCharacter::GetAmmoInventory()
{
	return AmmoInventory;
}

AGun* APlayerCharacter::GetCurrentWeapon()
{
	if (!CurrWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 장착된 총기 없음!!"));
	}

	return CurrWeapon;
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AItem* Item = Cast<AItem>(OtherActor);
		if (Item)
		{
			OverlappingItemList.Add(Item);
			UE_LOG(LogTemp, Warning, TEXT("주울 수 있는 아이템 : %s"), *Item->GetName());
		}
	}
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappingItemList.Num() > 0)
	{
		for (AItem* Item : OverlappingItemList)
		{
			if (OtherActor == Item)
			{
				OverlappingItemList.Remove(Item);
				return;
			}
		}
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//기본 무기인 권총 세팅
	EquipWeapon(EGunType::PISTOL);

	//힐포션 확인용 임시
	HealPotion = GetWorld()->SpawnActor<AHealingItem>(AHealingItem::StaticClass());
}

void APlayerCharacter::ChangeState(EPlayerStatus State)
{
	UE_LOG(LogTemp, Warning, TEXT("Change State!!"));
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = DamageAmount;

	if (ShieldGauge > 0)
	{
		ShieldGauge -= DamageAmount;

		if (ShieldGauge < 0)
		{
			Damage = ShieldGauge * -1;
			ShieldGauge = 0;
		}
	}

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// UI 업데이트

	return Damage;
}

void APlayerCharacter::StartAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Gun Fire!!"));
	if(CurrWeapon)
	{
		switch (CurrWeapon->GetGunType())
		{
		case RIFLE:
			Cast<ARifle>(CurrWeapon)->StartAutoFire();
			break;
		default:
			CurrWeapon->Fire();
			break;
		}
	}
}

void APlayerCharacter::StopAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Fire!!"));
	if (CurrWeapon)
	{
		if (ARifle* Rifle = Cast<ARifle>(CurrWeapon))
		{
			Rifle->StopAutoFire();
		}
	}
}

void APlayerCharacter::ReloadAmmo()
{
	//아이템 탄약 확인해서 장전하기 구현해야함

	if (CurrWeapon)
	{
		CurrWeapon->Reload();
	}
}

void APlayerCharacter::UseItem()
{
	HealPotion->Use();
}

void APlayerCharacter::AddItem(AItem* Item)
{
	AGun* Gun = Cast<AGun>(Item);
	if (Gun)
	{
		TArray<EGunType> GunList = GunManager->GetOwnedGunList();
		if (GunList.Find(Gun->GetGunType()) != INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("이미 소지한 총입니다!"));
			return;
		}
		GunManager->UpdateGunData(Gun);
	}
	else
	{
		AMagazine* Magazine = Cast<AMagazine>(Item);
		EItemType ItemType;
		if (Magazine)
		{
			ItemType = Magazine->GetAmmoType();
			int32 ItemCnt = 0;
			if (AmmoInventory.Find(ItemType))
			{
				ItemCnt = *AmmoInventory.Find(ItemType);
			}
			AmmoInventory.Add({ ItemType, ItemCnt + Magazine->GetAmmoAmount() });
		}
		else
		{
			ItemType = Item->GetItemType();
			switch (ItemType)
			{
			case SHIELD:
				Item->Use();
				break;
			case HEALINGITEM:
				HealPotion->IncrementCount(1);
				break;
			default:
				break;
			}
		}
	}
}

void APlayerCharacter::EquipWeapon(EGunType GunType)
{
	UClass* GunClass = nullptr;

	if (CurrWeapon)
	{
		GunManager->UpdateGunData(CurrWeapon);
		CurrWeapon->Destroy();
		CurrWeapon = nullptr;
	}

	switch (GunType)
	{
	case EGunType::PISTOL:
		GunClass = LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Pistol.BP_Pistol_C"));
		break;
	case EGunType::RIFLE:
		GunClass = LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Rifle.BP_Rifle_C"));
		break;
	case EGunType::SHOTGUN:
		GunClass = LoadClass<AGun>(nullptr, TEXT("/Game/Items/Blueprints/BP_Shotgun.BP_Shotgun_C"));
		break;
	default:
		break;
	}

	if(GetWorld()&&GunClass)
	{
		CurrWeapon = GetWorld()->SpawnActor<AGun>(GunClass);
	}

	if (CurrWeapon)
	{
		FName GunSocketName = "GunSocket_R";
		if (GetMesh()->DoesSocketExist(GunSocketName))
		{
			CurrWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GunSocketName);
		}
		CurrWeapon->SetOwner(this);
		GunManager->SetCurrentGun(CurrWeapon);
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
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::StartAttack
				);

				EnhancedInput->BindAction(
					PlayerController->AttackAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopAttack
				);

				EnhancedInput->BindAction(
					PlayerController->AttackAction,
					ETriggerEvent::Canceled,
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
					&APlayerCharacter::SelectWeapon
				);
			}

			if (PlayerController->PickupAction)
			{
				EnhancedInput->BindAction(
					PlayerController->PickupAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::PickupItem
				);
			}

			if (PlayerController->UseItemAction)
			{
				EnhancedInput->BindAction(
					PlayerController->UseItemAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::UseItem
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

void APlayerCharacter::SelectWeapon(const FInputActionValue& Value)
{
	int32 SelectInput = Value.Get<float>();
	TArray<EGunType> GunList = GunManager->GetOwnedGunList();

	if (GunList.Num()<=1)
	{
		UE_LOG(LogTemp, Warning, TEXT("장착할 수 있는 다른 총이 없습니다."));
		for (auto a : GunList)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d"), a);
		}
		return;
	}

	int32 GunListIdx = GunList.Find(CurrWeapon->GetGunType()) + SelectInput;
	int32 LastIdx = GunList.Num() - 1;

	if (GunListIdx < 0)
	{
		GunListIdx = LastIdx;
	}
	else if (GunListIdx > LastIdx)
	{
		GunListIdx = 0;
	}

	EquipWeapon(GunList[GunListIdx]);
}

void APlayerCharacter::PickupItem(const FInputActionValue& Value)
{
	if (!OverlappingItemList.IsEmpty())
	{
		AItem* OverlappingItem = OverlappingItemList[0];
		OverlappingItemList.RemoveAt(0);
		AddItem(OverlappingItem);
		UE_LOG(LogTemp, Warning, TEXT("%s 획득!"), *OverlappingItem->GetName());
		OverlappingItem->Destroy();
	}
}

