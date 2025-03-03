#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Shield.generated.h"

UCLASS()
class LOSTINEDEN_API AShield : public AItem
{
    GENERATED_BODY()

public:
    AShield();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
    float ShieldAmount; // 

    virtual void Use() override; 
};
