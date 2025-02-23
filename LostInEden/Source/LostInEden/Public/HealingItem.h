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
    float HealAmount; // ✅ 회복량

    virtual void Use() override; // ✅ 아이템 사용 오버라이드
};
