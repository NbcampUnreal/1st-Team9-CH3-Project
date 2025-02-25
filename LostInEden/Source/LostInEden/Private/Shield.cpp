#include "Shield.h"
#include "PlayerCharacter.h"

AShield::AShield()
{
    ItemName = "Shield";
    ItemDescription = "Increases player's defense.";
    ShieldAmount = 25.0f; // 기본 방어력 증가량 25
}

void AShield::Use()
{
   APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()); // 플레이어 가져오기
   if (Player)
   {
        
        UE_LOG(LogTemp, Log, TEXT("%s used, gaining %f shield"), *ItemName, ShieldAmount);
    }
}
