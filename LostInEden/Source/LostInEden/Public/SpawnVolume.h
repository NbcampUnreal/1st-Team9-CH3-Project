// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class LOSTINEDEN_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

protected:
	virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    class UBoxComponent* SpawningBox;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    FVector GetRandomPointInVolume() const;
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass);
};
