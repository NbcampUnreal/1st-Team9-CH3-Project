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
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()); 
    if (Player)
    {
       // Player->Health = FMath::Clamp(Player->Health + 40, 0, Player->MaxHealth);
    }
}
