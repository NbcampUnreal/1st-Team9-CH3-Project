// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"

ABaseAIController::ABaseAIController()
{

}

void ABaseAIController::OnPossess(APawn* InPawn)
{}
void ABaseAIController::BeginPlay()
{}
void ABaseAIController::SetStateAsPassive()
{}
void ABaseAIController::SetStateAsAttacking(AActor* AttackTarget)
{}
void ABaseAIController::SetStateAsInvestiagtion(FVector* Location)
{}
void ABaseAIController::SetStateAsStunned()
{}
void ABaseAIController::SetStateAsDead()
{}
void ABaseAIController::CanSenseActor()
{}
void ABaseAIController::HandleSensedSight()
{}
void ABaseAIController::HandleSensedSound()
{}
void ABaseAIController::HandleSensedDamage()
{}
void ABaseAIController::GetCurrentState()
{}