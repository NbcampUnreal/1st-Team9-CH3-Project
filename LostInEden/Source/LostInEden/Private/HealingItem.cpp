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
        int32 NewHealthValue = FMath::Clamp(Player->GetHealth() + HealAmount, 0, Player->GetMaxHealth());

        Player->SetHealth(NewHealthValue);


        Destroy();
    }
}
