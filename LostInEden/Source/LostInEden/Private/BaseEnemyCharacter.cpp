// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyCharacter.h"
#include "Entity.h"
#include "Components/WidgetComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"


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
void ABaseEnemyCharacter::UpdateHealth()
{
	if (IsValid(HealthBarWidget))
	{
		GetUserwi
	}
}
void ABaseEnemyCharacter::OnDead()
{
	// ai ��Ʈ�ѷ� �극�� ������Ʈ ����
	// ���� �ùķ��̼�, �浹 ��Ȱ��ȭ
	// ���¸� dead�� ����
	// ��
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