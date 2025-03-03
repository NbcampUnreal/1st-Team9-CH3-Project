#include "HealingItem.h"
#include "PlayerCharacter.h"

AHealingItem::AHealingItem()
{
    ItemName = EItemType::HEALINGITEM;
    ItemDescription = "Restores health when used.";
    HealAmount = 40.0f;
    ItemType = EItemType::HEALINGITEM; 
}
void AHealingItem::Use()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()); 
    if (Player)
    {
       // Player->Health = FMath::Clamp(Player->Health + 40, 0, Player->MaxHealth);
    }
}
