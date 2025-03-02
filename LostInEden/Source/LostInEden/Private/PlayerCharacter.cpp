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
	// 기본 권총 장착
	CurrWeapon = { EGunType::PISTOL, nullptr };

	// 오버랩된 아이템 초기화
	OverlappingItem = nullptr;
}

int32 APlayerCharacter::GetShieldGauge() const
{
	return ShieldGauge;
}

int32 APlayerCharacter::GetMaxShieldGauge() const
{
	return MaxShieldGauge;
}

AGun* APlayerCharacter::GetCurrentWeapon()
{
	if (!CurrWeapon.Value)
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 장착된 총기 없음!!"));
	}

	return CurrWeapon.Value;
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		OverlappingItem = Cast<AItem>(OtherActor);
		if (OverlappingItem)
		{
			UE_LOG(LogTemp, Warning, TEXT("주울 수 있는 아이템 : %s"), *OverlappingItem->GetName());
		}
	}
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OverlappingItem)
	{
		if (OtherActor == OverlappingItem)
		{
			OverlappingItem = nullptr;
		}
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(CurrWeapon.Key);
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
	AGun* BP_Weapon = CurrWeapon.Value;
	if(BP_Weapon)
	{
		switch (CurrWeapon.Key)
		{
		case RIFLE:
			Cast<ARifle>(BP_Weapon)->StartAutoFire();
			break;
		default:
			BP_Weapon->Fire();
			break;
		}
	}
}

void APlayerCharacter::StopAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Fire!!"));
	AGun* BP_Weapon = CurrWeapon.Value;
	if (BP_Weapon)
	{
		if (ARifle* Rifle = Cast<ARifle>(BP_Weapon))
		{
			Rifle->StopAutoFire();
		}
	}
}

void APlayerCharacter::ReloadAmmo()
{
	//아이템 탄약 확인해서 장전하기 구현해야함

	AGun* BP_Weapon = CurrWeapon.Value;
	if (BP_Weapon)
	{
		BP_Weapon->Reload();
	}
}

void APlayerCharacter::UseItem(EItemType ItemType)
{
	UE_LOG(LogTemp, Warning, TEXT("Use Item!!"));
}

void APlayerCharacter::AddItem(EItemType ItemType)
{
	switch (ItemType)
	{
	case SHIELD:
		ItemInventory.Add({ ItemType, 1 });
		break;
	case HEALINGITEM:
		ItemInventory.Add({ ItemType, 1 });
		break;
	default:
		break;
	}
}

void APlayerCharacter::EquipWeapon(EGunType GunType)
{
	UClass* GunClass = nullptr;

	if (CurrWeapon.Value)
	{
		//현재 총기의 정보 저장하는 코드 추가
		GunManager->UpdateGunData(CurrWeapon);
		CurrWeapon.Value->Destroy();
		CurrWeapon.Value = nullptr;
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
		CurrWeapon.Value = GetWorld()->SpawnActor<AGun>(GunClass);
	}

	if (CurrWeapon.Value)
	{
		FName GunSocketName = "GunSocket_R";
		if (GetMesh()->DoesSocketExist(GunSocketName))
		{
			CurrWeapon.Value->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GunSocketName);
		}
		CurrWeapon.Value->SetOwner(this);
		CurrWeapon.Key = GunType;
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

	int32 GunListIdx = GunList.Find(CurrWeapon.Key) + SelectInput;
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
	if (OverlappingItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 획득!"), *OverlappingItem->GetName());
		OverlappingItem->Destroy();
	}
}

