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

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    UMaterialInterface* PistolOutlineMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    UMaterialInterface* RifleOutlineMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    UMaterialInterface* ShotgunOutlineMaterial;


    void ApplyOutlineMaterial();

    UFUNCTION(BlueprintCallable, Category = "Magazine")
    EItemType GetAmmoType() const;

    UFUNCTION(BlueprintCallable, Category = "Magazine")
    int32 GetAmmoAmount() const;


    virtual void BeginPlay() override;
    virtual void Use(class APlayerCharacter*) override; 
    virtual void Tick(float DeltaTime) override;
};
