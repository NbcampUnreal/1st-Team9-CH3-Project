
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class LOSTINEDEN_API APatrolPath : public AActor
{
	GENERATED_BODY()

public:
	APatrolPath();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Patrol")
	TArray<AActor*> Waypoints;

	AActor* GetWaypoint(int32 index) const;
	int32 Num() const;
};
