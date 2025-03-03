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

	// 순찰경로. 레벨에서 직접 지정.
	PatrolRoute = nullptr;

	// 착용할 무기, 피격 모션은 자식 클래스에서 별도 지정
	WeaponActor = nullptr;
	HitReactionMontage = nullptr;

	// BeginPlay에서 받아옴
	MaterialInstance0 = nullptr;
	MaterialInstance1 = nullptr;
	
	// 체력바 표시. 표시될 체력바 위젯은 직접 지정
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
	
	// 동적 머티리얼 인스턴스 할당. OnDead에서 파라미터가 조절됩니다.
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
	// 체력 검사를 해서 죽으면 ondead 호출
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
	// ai 컨트롤러 브레인 컴포넌트 정지
	// 물리 시뮬레이션, 충돌 비활성화
	// 상태를 dead로 변경
	// 인
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
	//순찰 태스크에서 쓰이는 순찰루트의 getter
	return PatrolRoute;
}