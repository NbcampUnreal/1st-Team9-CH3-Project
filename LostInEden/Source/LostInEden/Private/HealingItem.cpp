#include "HealingItem.h"
//#include "MyCharacter.h"

AHealingItem::AHealingItem()
{
    ItemName = "Healing Potion";
    ItemDescription = "Restores health when used.";
    HealAmount = 50.0f; // 기본 회복량 50
}

void AHealingItem::Use()
{
   // AMyCharacter* Player = Cast<AMyCharacter>(GetOwner()); // 플레이어 가져오기
    //if (Player)
   // {
        // ✅ 여기서 플레이어의 체력을 회복하는 로직 추가
   //     UE_LOG(LogTemp, Log, TEXT("%s used, restoring %f HP"), *ItemName, HealAmount);
   // }
}
