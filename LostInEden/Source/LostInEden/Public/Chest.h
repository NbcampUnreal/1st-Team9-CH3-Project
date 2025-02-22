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

private:
    UPROPERTY(VisibleAnywhere, Category = "Chest")
    class USkeletalMeshComponent* ChestMesh;

    UPROPERTY(VisibleAnywhere, Category = "Chest")
    class UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, Category = "Chest|Animation")
    class UAnimationAsset* OpenAnim;

    UPROPERTY(EditAnywhere, Category = "Chest|Sound")
    class USoundBase* OpenSound;

    UPROPERTY(EditAnywhere, Category = "Chest|Sound")
    class USoundBase* CloseSound;

    UPROPERTY(EditAnywhere, Category = "Chest")
    float CloseDelay = 3.0f;  // 닫히는 시간 설정

    bool bIsOpened = false;   // 현재 열린 상태
    bool bIsClosing = false;  // 닫히는 중인지 여부
    bool bPlayerInTrigger = false; // ✅ 플레이어가 트리거 박스 안에 있는지 여부

    FTimerHandle CloseTimerHandle;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void OpenChest();
    void CloseChest();
   // void GiveItemsToPlayer(AActor* PlayerActor); // ✅ 플레이어에게 아이템 지급 함수 추가
};
