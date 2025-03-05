#include "HealingItem.h"
#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AHealingItem::AHealingItem()
{
    ItemName = EItemType::HEALINGITEM;
    ItemDescription = "Restores health when used.";
    HealAmount = 40.0f;
    ItemType = EItemType::HEALINGITEM;

    // HealingMesh 초기화
    HealingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealingMesh"));
    RootComponent = HealingMesh;

    // Static Mesh 로드
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMesh(TEXT("/Game/Scarlet_Elixir_1114043200_fbx/Scarlet_Elixir_1114043200.Scarlet_Elixir_1114043200"));
    if (PotionMesh.Succeeded())
    {
        HealingMesh->SetStaticMesh(PotionMesh.Object);
    }

}

void AHealingItem::BeginPlay()
{
    Super::BeginPlay();
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

void AHealingItem::IncrementCount(int32 Amount)
{
    Count += Amount;
}


EItemType AHealingItem::GetItemType() const
{
    return ItemType;
}

int32 AHealingItem::GetHealAmount() const
{
    return HealAmount;
}
