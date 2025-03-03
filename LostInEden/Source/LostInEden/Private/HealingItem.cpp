#include "HealingItem.h"
#include "PlayerCharacter.h"

AHealingItem::AHealingItem()
{
    ItemName = EItemType::HEALINGITEM;
    ItemDescription = "Restores health when used.";
    HealAmount = 40.0f;
    ItemType = EItemType::HEALINGITEM; 
}

void AHealingItem::IncrementCount(int32 Amount)
{
    Count += Amount;
}


void AHealingItem::Use()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
    if (Player && Count > 0)  
    {
        int32 NewHealth = Player->GetHealth() + HealAmount;
        Player->SetHealth(NewHealth); 

        Count--;  

        if (Count <= 0)
        {
            Destroy();
        }
    }
}

