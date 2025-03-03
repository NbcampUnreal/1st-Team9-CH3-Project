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
       // Player->ShieldGauge = FMath::Clamp(Player->ShieldGauge + ShieldAmount, 0, Player->GetMaxShieldGauge());

    }
}

