#include "Item.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AItem::Use()
{
    //UE_LOG(LogTemp, Warning, TEXT("%s was used!"), *ItemName);
}

EItemType AItem::GetItemName() const
{
    return ItemName;
}

EItemType AItem::GetItemType() const
{
    return ItemType; 
}
