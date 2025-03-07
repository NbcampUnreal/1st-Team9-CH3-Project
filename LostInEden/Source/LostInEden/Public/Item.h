#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IUsable.h"
#include "GlobalEnum.h"  // ✅ EItemType이 정의된 헤더 포함
#include "Item.generated.h"

UCLASS()
class LOSTINEDEN_API AItem : public AActor, public IIUsable
{
    GENERATED_BODY()

public:
    AItem();

    EItemType ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemDescription;

    EItemType ItemType;

    UFUNCTION(BlueprintCallable, Category = "Item")
    EItemType GetItemName() const;

    UFUNCTION(BlueprintCallable, Category = "Item")
    EItemType GetItemType() const;

    virtual void Use(class APlayerCharacter*) override;
};
