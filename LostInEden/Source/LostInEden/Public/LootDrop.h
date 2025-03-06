
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "LootDrop.generated.h"

UCLASS()
class LOSTINEDEN_API ALootDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	ALootDrop();

	UFUNCTION(BlueprintCallable, Category = "LootDrop")
	void SpawnRandomItem();
	void DestroyActor();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LootDrop")
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootDrop")
	UDataTable* ItemDataTable;

	FItemSpawnRow* GetRandomItem() const;
	void SpawnItem(TSubclassOf<AActor> ItemClass);

	// 아이템 발생 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootDrop")
	int LootChance;

	FTimerHandle DestroyTimerHandle;
};
