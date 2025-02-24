// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "AIController.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//빙의 시 초기화 작업

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	//if (Result.Code == EPathFollowingResult::Success)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Move is over. Going to next Point."));
	//	MoveToCurrentPatrolPoint();
	//}
}