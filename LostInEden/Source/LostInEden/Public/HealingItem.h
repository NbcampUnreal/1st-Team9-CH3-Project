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

    EItemType GetItemType() const;
    int32 GetHealAmount() const;

    virtual void Use() override; 
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    void ApplyOutlineMaterial();

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* HealingMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Healing|Material")
    UMaterialInterface* OutlineMaterial;
};
