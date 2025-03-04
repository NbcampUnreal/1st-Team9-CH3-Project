#include "Magazine.h"
#include "PlayerCharacter.h"

AMagazine::AMagazine()
{

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    MagazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineMesh"));
    MagazineMesh->SetupAttachment(RootComponent);

    ItemName = EItemType::NONE;
    ItemDescription = "A magazine containing ammunition.";
    AmmoAmount = 0; 
    AmmoType = EItemType::NONE;
}

void AMagazine::Use()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
    if (Player)
    {
        TMap<EItemType, int32>& Inventory = Player->GetAmmoInventory();

        if (Inventory.Contains(AmmoType))
        {
            Inventory[AmmoType] += AmmoAmount;
        }
        else
        {
            Inventory.Add(AmmoType, AmmoAmount);
        }

        UE_LOG(LogTemp, Warning, TEXT("%d발의 %s 탄약을 획득함! 현재 인벤토리: %d"),
            AmmoAmount, *UEnum::GetValueAsString(AmmoType), Inventory[AmmoType]);

        Destroy();
    }
}


EItemType AMagazine::GetAmmoType() const
{
    return AmmoType;
}
