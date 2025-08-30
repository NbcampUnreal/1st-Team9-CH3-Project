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

	GunManager = CreateDefaultSubobject<UGunManager>(TEXT("GunManager"));

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

	CurrWeapon = nullptr;

	ZoomInSound = LoadObject<USoundBase>(GetTransientPackage(), TEXT("/Game/Sounds/Lyra_ZoomIn_01.Lyra_ZoomIn_01"));
	if (ZoomInSound)
	{
		UE_LOG(LogTemp, Warning, TEXT("줌인 사운드 로드 완료!"));
	}
	
	ZoomOutSound = LoadObject<USoundBase>(GetTransientPackage(), TEXT("/Game/Sounds/Lyra_ZoomOut_01.Lyra_ZoomOut_01"));
	if (ZoomOutSound)
	{
		UE_LOG(LogTemp, Warning, TEXT("줌아웃 사운드 로드 완료!"));
	}

	bIsDead = false;
	bCanChangeGun = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//기본 권총 장착
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
	float Damage = 0;

	if (ShieldGauge > 0)
	{
		ShieldGauge -= DamageAmount;

		if (ShieldGauge < 0)
		{
			Damage = ShieldGauge * -1;
			ShieldGauge = 0;
		}
	}
	else
	{
		Damage = DamageAmount;
	}

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	UpdateUI();

	if (Health == 0)
	{
		bIsDead = true;
		DisableInput(Cast<APlayerController>(GetController()));
		GetWorldTimerManager().SetTimer(DelayDieState, this, &APlayerCharacter::Die, 2.5f, false);
	}

	return Damage;
}

void APlayerCharacter::StartAttack()
{
	bCanChangeGun = false;
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

void APlayerCharacter::PlayZoomInSound()
{
	if (ZoomInSound)
	{
		UGameplayStatics::PlaySound2D(this, ZoomInSound);
	}
}

void APlayerCharacter::PlayZoomOutSound()
{
	if (ZoomOutSound)
	{
		UGameplayStatics::PlaySound2D(this, ZoomOutSound);
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
	if (!GunManager)
	{
		UE_LOG(LogTemp, Error, TEXT("건매니저 없음!! - AddItem"));
		return;
	}

	if (Gun)
	{
		EGunType GunType = Gun->GetGunType();

		if (GunManager->HasWeapon(GunType))
		{
			UE_LOG(LogTemp, Warning, TEXT("이미 소지한 총입니다: %d"), GunType);
			return;
		}

		GunManager->AcquireWeapon(GunType);

		// 소유한 무기 목록 로그 출력
		TArray<EGunType> GunList = GunManager->GetOwnedGunList();
		UE_LOG(LogTemp, Warning, TEXT("업데이트 후 보유한 총 리스트:"));
		for (auto OwnedGunType : GunList)
		{
			UE_LOG(LogTemp, Warning, TEXT("GunType: %d"), OwnedGunType);
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

	if (!GunManager)
	{
		UE_LOG(LogTemp, Error, TEXT("건매니저 없음!! - EquipWeapon"));
		return;
	}

	if (!GunManager->HasWeapon(GunType))
	{
		UE_LOG(LogTemp, Warning, TEXT("소유하지 않은 무기입니다: %d"), GunType);
		return;
	}

	if (CurrWeapon)
	{
		CurrWeapon->SetActorHiddenInGame(true);
		CurrWeapon->SetActorEnableCollision(false);
		CurrWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	CurrWeapon = GunManager->GetWeapon(GunType);

	if (CurrWeapon)
	{
		CurrWeapon->SetActorHiddenInGame(false);
		CurrWeapon->SetActorEnableCollision(false);

		FName GunSocketName = (GunType == PISTOL) ? FName("Pistol_Socket") :
			(GunType == RIFLE) ? FName("Rifle_Socket") :
			(GunType == SHOTGUN) ? FName("Shotgun_Socket") : NAME_None;

		if (GetMesh()->DoesSocketExist(GunSocketName))
		{
			CurrWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GunSocketName);
		}

		CurrWeapon->SetOwner(this);
		UE_LOG(LogTemp, Warning, TEXT("무기 장착 완료: %d"), GunType);
		UpdateUI();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("무기 풀에서 무기를 찾을 수 없음: %d"), GunType);
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

			if (PlayerController->ZoomAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::PlayZoomInSound
				);

				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::PlayZoomOutSound
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
	if (!GunManager)
	{
		UE_LOG(LogTemp, Error, TEXT("건매니저 없음!! - SelectWeapon"));
		return;
	}

	const TArray<EGunType>& GunList = GunManager->GetOwnedGunList();

	if (GunList.Num() <= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("장착할 수 있는 다른 총이 없습니다."));
		return;
	}

	EGunType CurrentGunType = CurrWeapon ? CurrWeapon->GetGunType() : EGunType::PISTOL;
	int32 CurrentIndex = GunList.Find(CurrentGunType);
	if (CurrentIndex == INDEX_NONE)
	{
		CurrentIndex = 0;
	}

	int32 Direction = FMath::RoundToInt(Value.Get<float>());
	int32 NextIndex = (CurrentIndex + Direction + GunList.Num()) % GunList.Num();

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

