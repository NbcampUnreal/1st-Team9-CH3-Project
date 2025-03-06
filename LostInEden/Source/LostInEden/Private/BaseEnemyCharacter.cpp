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

	// 체력 검사를 해서 죽으면 ondead 호출
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
	
	// 1.ai 컨트롤러 브레인 컴포넌트 정지
	// 2.물리 시뮬레이션, 이동 중지, 충돌 비활성화, 캡슐 컴포넌트 최소화
	// 3.상태를 dead로 변경
	// 4.소멸 애니메이션 재생 후 destroy
	AAIController* AIController = Cast<AAIController>(GetController());
	if(!AIController) UE_LOG(LogTemp, Warning, TEXT("Contoller Failed"));
	if (AIController)
	{
		ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);
		if (!BaseAIController) UE_LOG(LogTemp, Warning, TEXT("Cast Failed"));
		if(BaseAIController)
		{
			// 1. 브레인 컴포넌트 정지
			UBrainComponent* BrainComponent = BaseAIController->GetBrainComponent();
			if (!BrainComponent) UE_LOG(LogTemp, Warning, TEXT("Component Failed"));
			if (BrainComponent)
			{
				BrainComponent->StopLogic("Character is Dead.");
			}
			UE_LOG(LogTemp, Warning, TEXT("On dead"));
			// 2. 물리 시뮬레이션 활성화, 이동 중지, 충돌 비활성화, 캡슐 컴포넌트 최소화
			GetMesh()->SetSimulatePhysics(true);
			GetCharacterMovement()->StopMovementImmediately();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetCapsuleComponent()->SetCapsuleSize(0.0f, 0.0f);
			

			// 3. 상태 변경
			BaseAIController->SetStateAsDead();

			// 4. 소멸 애니메이션 재생 후 destroy
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
	//순찰 태스크에서 쓰이는 순찰루트의 getter
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
	// 머티리얼 파라미터 업데이트
	if (MaterialInstance0)
	{
		MaterialInstance0->SetScalarParameterValue(TEXT("Amount"), LerpedValue);
	}
	if (MaterialInstance1)
	{
		MaterialInstance1->SetScalarParameterValue(TEXT("Amount"), LerpedValue);
	}

	// 나이아가라 변수 업데이트
	if (NiagaraComponent)
	{
		NiagaraComponent->SetNiagaraVariableFloat(TEXT("User.Amount"), LerpedValue);
	}
}