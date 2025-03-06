// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyCharacter.h"
#include "Entity.h"
#include "Components/WidgetComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "AIController.h"
#include "BaseAIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

class UWBP_HealthBar;

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MaxHealth = 100;
	Health = MaxHealth;
	IsWieldingWeapon = false;
	IsDead = false;

	// �������. �������� ���� ����.
	PatrolRoute = nullptr;

	// ������ ����, �ǰ� ����� �ڽ� Ŭ�������� ���� ����
	WeaponActor = nullptr;
	HitReactionMontage = nullptr;

	// BeginPlay���� �޾ƿ�
	MaterialInstance0 = nullptr;
	MaterialInstance1 = nullptr;
	
	// ü�¹� ǥ��. ǥ�õ� ü�¹� ������ ���� ����
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetMesh());
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(150, 20));

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OnDeadVFX"));
	NiagaraComponent->SetupAttachment(RootComponent);

}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// ���� ��Ƽ���� �ν��Ͻ� �Ҵ�. OnDead���� �Ķ���Ͱ� �����˴ϴ�.
	if (GetMesh())
	{
		MaterialInstance0 = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0));
		MaterialInstance1 = GetMesh()->CreateDynamicMaterialInstance(1, GetMesh()->GetMaterial(1));
	}

}

void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLerping)
	{
		ElapsedTime += DeltaTime;
		CurrentAlpha = FMath::Clamp((LerpDuration -ElapsedTime) / LerpDuration, 0.0f, 1.0f);
		const float LerpedValue = FMath::Clamp(Lerp_A, Lerp_B, CurrentAlpha);
		UpdateLerpedValues(LerpedValue);

		if (CurrentAlpha >= 1.0f)
		{
			bIsLerping = false;
		}
	}

}
float ABaseEnemyCharacter::TakeDamage(float AmountDamage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = AEntity::TakeDamage(AmountDamage, DamageEvent, EventInstigator, DamageCauser);

	// ü�� �˻縦 �ؼ� ������ ondead ȣ��
	if (Health <= 0)
	{
		IsDead = true;
		UE_LOG(LogTemp, Warning, TEXT("Enemy died"));
		OnDead();
	}
	else
	{
		OnStunned();
	}
	return Damage;
}
//void ABaseEnemyCharacter::UpdateHealth()
//{
//	if (!HealthBarWidget) return;
//	
//	UUserWidget* HealthBarWidgetInstance = HealthBarWidget->GetUserWidgetObject();
//	if (!HealthBarWidgetInstance) return;
//
//	UWBP_HealthBar* HealthBar = Cast<UWBP_HealthBar>(HealthBarWidgetInstance);
//	if (HealthBar)
//	{
//		
//	}
//
//}
void ABaseEnemyCharacter::OnDead()
{
	
	// 1.ai ��Ʈ�ѷ� �극�� ������Ʈ ����
	// 2.���� �ùķ��̼�, �̵� ����, �浹 ��Ȱ��ȭ, ĸ�� ������Ʈ �ּ�ȭ
	// 3.���¸� dead�� ����
	// 4.�Ҹ� �ִϸ��̼� ��� �� destroy
	AAIController* AIController = Cast<AAIController>(GetController());
	if(!AIController) UE_LOG(LogTemp, Warning, TEXT("Contoller Failed"));
	if (AIController)
	{
		ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);
		if (!BaseAIController) UE_LOG(LogTemp, Warning, TEXT("Cast Failed"));
		if(BaseAIController)
		{
			// 1. �극�� ������Ʈ ����
			UBrainComponent* BrainComponent = BaseAIController->GetBrainComponent();
			if (!BrainComponent) UE_LOG(LogTemp, Warning, TEXT("Component Failed"));
			if (BrainComponent)
			{
				BrainComponent->StopLogic("Character is Dead.");
			}
			UE_LOG(LogTemp, Warning, TEXT("On dead"));
			// 2. ���� �ùķ��̼� Ȱ��ȭ, �̵� ����, �浹 ��Ȱ��ȭ, ĸ�� ������Ʈ �ּ�ȭ
			GetMesh()->SetSimulatePhysics(true);
			GetCharacterMovement()->StopMovementImmediately();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetCapsuleComponent()->SetCapsuleSize(0.0f, 0.0f);
			

			// 3. ���� ����
			BaseAIController->SetStateAsDead();

			// 4. �Ҹ� �ִϸ��̼� ��� �� destroy
			GetWorldTimerManager().SetTimer(PlayDeadAnimTimerHandle, this, &ABaseEnemyCharacter::PlayDeadAnim, 1, false);
		}
	}
}
void ABaseEnemyCharacter::PlayDeadAnim()
{
	StartLerp(0.0f, 1.0f, 5.0f);
}
void ABaseEnemyCharacter::OnStunned()
{
	GetCharacterMovement()->StopMovementImmediately();
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);
		if (BaseAIController)
		{
			BaseAIController->SetStateAsStunned();
		}
	}
}

void ABaseEnemyCharacter::Heal(float HealPercentage)
{

}
void ABaseEnemyCharacter::Attack()
{

}
void ABaseEnemyCharacter::EquipWeapon()
{

}
void ABaseEnemyCharacter::UnequipWeapon() 
{

}
void ABaseEnemyCharacter::GetIdealRange(float& AttackRadius, float& DefendRadius)
{

}
void ABaseEnemyCharacter::SetMovementSpeed(E_MovementSpeed Speed, float& SpeedValue)
{

}
class AActor* ABaseEnemyCharacter::GetPatrolRoute() const
{
	//���� �½�ũ���� ���̴� ������Ʈ�� getter
	return PatrolRoute;
}

void ABaseEnemyCharacter::StartLerp(float InStartValue, float InEndValue, float InDuration)
{
	if (InDuration <= 0.0f) return;

	bIsLerping = true;
	Lerp_A = InStartValue;
	Lerp_B = InEndValue;
	LerpDuration = InDuration;
	ElapsedTime = 0.0f;
	CurrentAlpha = 0.0f;
}

void ABaseEnemyCharacter::UpdateLerpedValues(float LerpedValue)
{
	// ��Ƽ���� �Ķ���� ������Ʈ
	if (MaterialInstance0)
	{
		MaterialInstance0->SetScalarParameterValue(TEXT("Amount"), LerpedValue);
	}
	if (MaterialInstance1)
	{
		MaterialInstance1->SetScalarParameterValue(TEXT("Amount"), LerpedValue);
	}

	// ���̾ư��� ���� ������Ʈ
	if (NiagaraComponent)
	{
		NiagaraComponent->SetNiagaraVariableFloat(TEXT("User.Amount"), LerpedValue);
	}
}