#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HealingItem.generated.h"

UCLASS()
class LOSTINEDEN_API AHealingItem : public AItem
{
    GENERATED_BODY()

public:
    AHealingItem();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
    float HealAmount; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
    int32 Count;

    void IncrementCount(int32 Amount = 1); // 기본값 1로 설정

    virtual void Use() override; 
};
