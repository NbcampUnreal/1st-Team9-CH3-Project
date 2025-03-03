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
    if (Player)
    {
        int32 NewShieldValue = FMath::Clamp(Player->GetShieldGauge() + ShieldAmount, 0, Player->GetMaxShieldGauge());

        Player->SetShieldGauge(NewShieldValue);

        Destroy();
    }
}

