#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GlobalEnum.h"
#include "Magazine.generated.h"

UCLASS()
class LOSTINEDEN_API AMagazine : public AItem
{
    GENERATED_BODY()

public:
    AMagazine();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MagazineMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
    int32 AmmoAmount; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
    TEnumAsByte<EItemType> AmmoType;

    UFUNCTION(BlueprintCallable, Category = "Magazine")
    EItemType GetAmmoType() const;



    virtual void Use() override; 
};
