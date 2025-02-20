#include "Item.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AItem::Use()
{
    UE_LOG(LogTemp, Warning, TEXT("%s was used!"), *ItemName);
}
