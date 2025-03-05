
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EdenObjectPool.generated.h"

UCLASS()
class LOSTINEDEN_API AEdenObjectPool : public AActor
{
	GENERATED_BODY()
	
public:	
	AEdenObjectPool();

	UFUNCTION(BlueprintCallable)
	AActor* GetPooledObejct();
	UFUNCTION(BlueprintCallable)
	void ReturnPooledObject(AActor* PooledObject);
	UFUNCTION(BlueprintCallable)
	void Activate(AActor* PooledObject);
	UFUNCTION(BlueprintCallable)
	void Deactivate(AActor* PooledObject);

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TSubclassOf<AActor> ObjectToPool;
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int32 PoolSize;
	UPROPERTY(VisibleAnywhere, Category = "Object Pool")
	TArray<AActor*> PooledObjects;

};
