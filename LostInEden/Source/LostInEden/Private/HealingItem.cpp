#include "HealingItem.h"
#include "PlayerCharacter.h"

AHealingItem::AHealingItem()
{
    ItemName = "Healing Potion";
    ItemDescription = "Restores health when used.";
    HealAmount = 50.0f; 
}

void AHealingItem::Use()
{
   APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()); // 플레이어 가져오기
   if (Player)
   {  
      UE_LOG(LogTemp, Log, TEXT("%s used, restoring %f HP"), *ItemName, HealAmount);
   }
}
