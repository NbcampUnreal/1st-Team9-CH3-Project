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
    Count = 0;


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

void AHealingItem::Use(APlayerCharacter* Player)
{
    if (Player && Count > 0)
    {
        int32 NewHealth = Player->GetHealth() + HealAmount;
        NewHealth = FMath::Clamp(NewHealth, 0, Player->GetMaxHealth());
        Player->SetHealth(NewHealth);
        UE_LOG(LogTemp, Display, TEXT("체력 게이지 %d"), Player->GetMaxHealth());

        Count--;
    }
}

void AHealingItem::IncrementCount(int32 Amount)
{
    if (!this)
    {
        UE_LOG(LogTemp, Error, TEXT("IncrementCount(): AHealingItem 객체가 nullptr입니다!"));
        return;
    }

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

int32 AHealingItem::GetCount() const
{
    return Count;
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
