#include "HealingItem.h"
#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AHealingItem::AHealingItem()
{
    PrimaryActorTick.bCanEverTick = true;
    ItemName = EItemType::HEALINGITEM;
    ItemDescription = "Restores health when used.";
    HealAmount = 40.0f;
    ItemType = EItemType::HEALINGITEM;


    HealingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealingMesh"));
    RootComponent = HealingMesh;

 
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMesh(TEXT("/Game/Items/future-first-aid-kit/source/Medibox.Medibox"));
    if (PotionMesh.Succeeded())
    {
        HealingMesh->SetStaticMesh(PotionMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> OutlineMat(TEXT("/Game/Items/Material/POTION.POTION"));
    if (OutlineMat.Succeeded())
    {
        OutlineMaterial = OutlineMat.Object;
    }
}

void AHealingItem::BeginPlay()
{
    Super::BeginPlay();
    FVector NewLocation = GetActorLocation();
    NewLocation.Z += 30.0f;
    SetActorLocation(NewLocation);

    ApplyOutlineMaterial();
}

void AHealingItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += 100.0f * DeltaTime;  


    SetActorRotation(NewRotation);
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

void AHealingItem::ApplyOutlineMaterial()
{
    if (!HealingMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("ShieldMesh가 없음!"));
        return;
    }

    if (OutlineMaterial)
    {
        UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
        if (DynamicMat)
        {
            HealingMesh->SetOverlayMaterial(DynamicMat);

        }
    }
}
