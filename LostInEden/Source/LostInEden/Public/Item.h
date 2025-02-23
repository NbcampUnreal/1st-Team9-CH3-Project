// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IUsable.h"
#include "Item.generated.h"

UCLASS()
class LOSTINEDEN_API AItem : public AActor, public IIUsable
{
	GENERATED_BODY()
	
public:
    AItem();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemDescription;

    virtual void Use() override;

};
