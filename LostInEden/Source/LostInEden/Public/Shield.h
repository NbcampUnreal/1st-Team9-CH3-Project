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
    float ShieldAmount; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
    int32 Count;

    virtual void Use(class APlayerCharacter*) override;
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* ShieldMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialInterface* OutlineMaterial;

    void ApplyOutlineMaterial();

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* ShieldSound;
};
