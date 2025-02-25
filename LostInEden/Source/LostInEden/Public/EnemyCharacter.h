// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class LOSTINEDEN_API AEnemyCharacter : public AEntity
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Enemy Properties")
	int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	int32 Shield;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void ThrowGrenade();
	UFUNCTION(BlueprintCallable)
	void DropItem();
	UFUNCTION(BlueprintCallable)
	void RequestAid();
};
