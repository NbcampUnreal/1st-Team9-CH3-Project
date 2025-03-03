#include "Shield.h"
#include "PlayerCharacter.h"

AShield::AShield()
{
    ItemName = EItemType::SHIELD;
    ItemDescription = "Increases player's defense.";
    ShieldAmount = 25.0f; 
    ItemType = EItemType::SHIELD;
}

void AShield::Use()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
    if (Player && Count > 0) 
    {
        int32 NewShieldValue = Player->GetShieldGauge() + ShieldAmount;
        Player->SetShieldGauge(NewShieldValue);

        Count--; 

        if (Count <= 0)
        {
            Destroy(); 
        }
    }
}
