#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

UCLASS()
class LOSTINEDEN_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	void SetStateAsPassive();
	void SetStateAsAttacking(AActor* AttackTarget);
	void SetStateAsInvestiagtion(FVector* Location);
	void SetStateAsStunned();
	void SetStateAsDead();
	void CanSenseActor();
	void HandleSensedSight();
	void HandleSensedSound();
	void HandleSensedDamage();
	void GetCurrentState();


protected:
		
};