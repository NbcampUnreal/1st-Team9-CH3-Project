#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

UCLASS()
class LOSTINEDEN_API AChest : public AActor
{
    GENERATED_BODY()

public:
    AChest();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void OpenChest(); // ✅ E 키를 눌렀을 때 실행

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void CloseChest(); // ✅ UI 닫으면 실행

    bool bCanInteract = false; // ✅ 상호작용 가능 여부

private:
    UPROPERTY(VisibleAnywhere, Category = "Chest")
    class USkeletalMeshComponent* ChestMesh;

    UPROPERTY(VisibleAnywhere, Category = "Chest")
    class UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, Category = "Chest|Items")
    TArray<TSubclassOf<class AItem>> ChestItems; // ✅ 상자 아이템 리스트

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
