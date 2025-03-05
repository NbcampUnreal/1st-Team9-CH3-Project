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

class UWBP_HealthBar;

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
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

float ABaseEnemyCharacter::TakeDamage(float AmountDamage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = AEntity::TakeDamage(AmountDamage, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
	// ü�� �˻縦 �ؼ� ������ ondead ȣ��
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
	// 2.���� �ùķ��̼�, �浹 ��Ȱ��ȭ
	// 3.���¸� dead�� ����
	// 4.�Ҹ� �ִϸ��̼� ��� �� destroy
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);
		if(BaseAIController)
		{
			// 1. �극�� ������Ʈ ����
			UBrainComponent* BrainComponent = BaseAIController->GetBrainComponent();
			if (BrainComponent)
			{
				BrainComponent->StopLogic("Character is Dead.");
			}

			// 2. ���� �ùķ��̼� Ȱ��ȭ, �浹 ��Ȱ��ȭ
			GetMesh()->SetSimulatePhysics(true);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			// 3. ���� ����
			BaseAIController->SetStateAsDead();

			// 4. �Ҹ� �ִϸ��̼� ��� �� destroy
			GetWorldTimerManager().SetTimer(PlayDeadAnimTimerHandle, this, &ABaseEnemyCharacter::PlayDeadAnim, 1, false);
		}
	}
}
void ABaseEnemyCharacter::PlayDeadAnim()
{
	
}
void ABaseEnemyCharacter::OnStunned()
{

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