#include "Shield.h"
//#include "MyCharacter.h"

AShield::AShield()
{
    ItemName = "Shield";
    ItemDescription = "Increases player's defense.";
    ShieldAmount = 25.0f; // 기본 방어력 증가량 25
}

void AShield::Use()
{
   //AMyCharacter* Player = Cast<AMyCharacter>(GetOwner()); // 플레이어 가져오기
    //if (Player)
   // {
        // ✅ 여기서 플레이어의 방어력을 증가하는 로직 추가
   //     UE_LOG(LogTemp, Log, TEXT("%s used, gaining %f shield"), *ItemName, ShieldAmount);
   // }
}
