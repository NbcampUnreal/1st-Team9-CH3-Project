#include "TestCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"

ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 설정
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 입력 매핑 컨텍스트 설정
	auto PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(imc_TPS, 0);
		}
	}

	// 무기 장착
	EquipGun();
}

void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &ATestCharacter::Move);
		PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &ATestCharacter::Turn);
		PlayerInput->BindAction(ia_Lookup, ETriggerEvent::Triggered, this, &ATestCharacter::LookUp);
		PlayerInput->BindAction(ia_Fire, ETriggerEvent::Started, this, &ATestCharacter::Fire);
	}
}

void ATestCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D InputAxis = InputValue.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), InputAxis.Y);
	AddMovementInput(GetActorRightVector(), InputAxis.X);
}

void ATestCharacter::Turn(const FInputActionValue& InputValue)
{
	float AxisValue = InputValue.Get<float>();
	AddControllerYawInput(AxisValue);
}

void ATestCharacter::LookUp(const FInputActionValue& InputValue)
{
	float AxisValue = InputValue.Get<float>();
	AddControllerPitchInput(AxisValue);
}

void ATestCharacter::Fire()
{
	if (EquippedGun)
	{
		EquippedGun->Fire();
	}
}

void ATestCharacter::Reload()
{
	if (EquippedGun)
	{
		EquippedGun->Reload();
	}
}

void ATestCharacter::EquipGun()
{
	if (RifleClass)
	{
		EquippedGun = GetWorld()->SpawnActor<ARifle>(RifleClass);
		if (EquippedGun)
		{
			EquippedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GunSocket");
			EquippedGun->SetOwner(this);
		}
	}
}
