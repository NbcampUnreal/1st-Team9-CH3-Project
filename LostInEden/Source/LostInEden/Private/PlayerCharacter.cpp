// Copyright Epic Games, Inc. All Rights Reserved.
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GunManager.h"
#include "Gun.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Shotgun.h"
#include "Shield.h"
#include "HealingItem.h"
#include "Magazine.h"
#include "EdenGameState.h"

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

	// 오버랩 이벤트 바인딩
	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

	NormalSpeed = 600.f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	MaxHealth = 200;
	Health = MaxHealth;

	MaxShieldGauge = 50;
	ShieldGauge = 0;

	// 건 매니저 생성
	GunManager = CreateDefaultSubobject<UGunManager>(TEXT("GunManager"));
	CurrWeapon = nullptr;

	bIsDead = false;
	bCanChangeGun = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(EGunType::PISTOL);

	HealPotion = GetWorld()->SpawnActor<AHealingItem>(AHealingItem::StaticClass());
	Shield = GetWorld()->SpawnActor<AShield>(AShield::StaticClass());
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

int32 APlayerCharacter::GetHealPotionCnt() const
{
	if (!HealPotion)
	{
		UE_LOG(LogTemp, Warning, TEXT("힐링 아이템이 없습니다!"));
		return 0;
	}
	return HealPotion->GetCount();
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

	UpdateUI();

	if (Health == 0)
	{
		bIsDead = true;
		DisableInput(Cast<APlayerController>(GetController()));
		Die();
	}

	return Damage;
}

void APlayerCharacter::StartAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Gun Fire!!"));

	if (CurrWeapon)
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

	UpdateUI();

	// 공격 도중에도 무기 변경 가능하게 유지
	bCanChangeGun = true;
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

	UpdateUI();

	// 공격 종료 후 무기 변경 가능하게 설정
	bCanChangeGun = true;
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
	if (!HealPotion)
	{
		UE_LOG(LogTemp, Warning, TEXT("UseItem(): HealPotion이 없습니다. 다시 생성 시도!"));
		HealPotion = GetWorld()->SpawnActor<AHealingItem>(AHealingItem::StaticClass());

		if (!HealPotion)
		{
			UE_LOG(LogTemp, Error, TEXT("UseItem(): HealPotion 재생성 실패!"));
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UseItem(): HealPotion 사용!"));
	HealPotion->Use(this);
	UpdateUI();
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

		UE_LOG(LogTemp, Warning, TEXT("새로운 총 획득: %d"), Gun->GetGunType());
		GunManager->UpdateGunData(Gun); // 무기 데이터 업데이트

		// 보유한 총 리스트 다시 확인
		GunList = GunManager->GetOwnedGunList();
		UE_LOG(LogTemp, Warning, TEXT("업데이트 후 보유한 총 리스트:"));
		for (auto GunType : GunList)
		{
			UE_LOG(LogTemp, Warning, TEXT("GunType: %d"), GunType);
		}
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
				if (!Shield)
				{
					UE_LOG(LogTemp, Warning, TEXT("Shield가 nullptr입니다. 다시 생성 시도!"));
					Shield = GetWorld()->SpawnActor<AShield>(AShield::StaticClass());

					if (!Shield)
					{
						UE_LOG(LogTemp, Error, TEXT("Shield 생성 실패!"));
						break;
					}
				}

				Shield->Use(this);
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

	if (GetWorld() && GunClass)
	{
		CurrWeapon = GetWorld()->SpawnActor<AGun>(GunClass);
		CurrWeapon->SetActorEnableCollision(false);
	}

	if (CurrWeapon)
	{
		FName GunSocketName;
		switch (GunType)
		{
		case PISTOL:
			GunSocketName = "Pistol_Socket";
			break;
		case RIFLE:
			GunSocketName = "Rifle_Socket";
			break;
		case SHOTGUN:
			GunSocketName = "Shotgun_Socket";
			break;
		default:
			break;
		}

		if (GetMesh()->DoesSocketExist(GunSocketName))
		{
			CurrWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GunSocketName);
		}
		CurrWeapon->SetOwner(this);
		GunManager->SetCurrentGun(CurrWeapon);
		UpdateUI();
	}
}

void APlayerCharacter::ResetInput()
{
	bCanChangeGun = true;
}

void APlayerCharacter::Die()
{
    // 현재 게임의 GameState 가져오기
    AEdenGameState* GameState = Cast<AEdenGameState>(UGameplayStatics::GetGameState(this));
    if (GameState)
    {
        GameState->OnGameOver();  // GameState에 사망 알리기
    }
}

void APlayerCharacter::UpdateUI()
{
	// UI 업데이트

	// 컨트롤러의 HUD 업데이트 함수 호출
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetController());
	if (PC)
	{
		PC->UpdateHUD();
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
	if (!bCanChangeGun)
	{
		UE_LOG(LogTemp, Warning, TEXT("can't change gun!!"));
		return;
	}

	TArray<EGunType> GunList = GunManager->GetOwnedGunList();

	// 총이 2개 이상 있어야 교체 가능
	if (GunList.Num() <= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("장착할 수 있는 다른 총이 없습니다."));
		return;
	}

	// 현재 무기의 인덱스 찾기
	int32 CurrentIndex = GunList.Find(CurrWeapon->GetGunType());
	if (CurrentIndex == INDEX_NONE)
	{
		// 현재 무기가 GunList에 없다면, 일단 0으로 설정
		CurrentIndex = 0;
	}

	// 입력값 (일반적으로 1이나 -1을 기대)
	int32 Direction = FMath::RoundToInt(Value.Get<float>());

	// 새 인덱스 계산: (현재 인덱스 + 방향 + 전체개수) % 전체개수
	int32 NextIndex = (CurrentIndex + Direction + GunList.Num()) % GunList.Num();

	// 새 인덱스 무기 장착
	EquipWeapon(GunList[NextIndex]);

	bCanChangeGun = false;
	GetWorldTimerManager().SetTimer(InputDelayTimerHandle, this, &APlayerCharacter::ResetInput, 2.0f, false);
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

	UpdateUI();
}

