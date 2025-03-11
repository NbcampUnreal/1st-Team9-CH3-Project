#include "Shield.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AShield::AShield()
{
    PrimaryActorTick.bCanEverTick = true;
    ItemName = EItemType::SHIELD;
    ItemDescription = "Increases player's defense.";
    ShieldAmount = 25.0f; 
    ItemType = EItemType::SHIELD;

    ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
    RootComponent = ShieldMesh;


    static ConstructorHelpers::FObjectFinder<UStaticMesh> ShieldMeshAsset(TEXT("/Game/Items/wooden-medieval-shield/source/Shield.Shield"));
    if (ShieldMeshAsset.Succeeded())
    {
        ShieldMesh->SetStaticMesh(ShieldMeshAsset.Object);
    }


    static ConstructorHelpers::FObjectFinder<UMaterialInterface> OutlineMat(TEXT("/Game/Items/Material/SheildTexture.SheildTexture"));
    if (OutlineMat.Succeeded())
    {
        OutlineMaterial = OutlineMat.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> SoundObj(TEXT("/Game/Interface_And_Item_Sounds/Cues/Item_Sell_Purchase_05_Cue.Item_Sell_Purchase_05_Cue"));
    if (SoundObj.Succeeded())
    {
        ShieldSound = SoundObj.Object;
    }
}

void AShield::Use(APlayerCharacter* Player)
{
    if (Player)
    {
        int32 NewShieldValue = Player->GetShieldGauge() + ShieldAmount;
        NewShieldValue = FMath::Clamp(NewShieldValue, 0, Player->GetMaxShieldGauge());
        Player->SetShieldGauge(NewShieldValue);

        UE_LOG(LogTemp, Display, TEXT("쉴드 게이지 %d"), Player->GetMaxShieldGauge());

        if (ShieldSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, ShieldSound, GetActorLocation());
        }
    }
}


void AShield::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += 100.0f * DeltaTime;


    SetActorRotation(NewRotation);
}


void AShield::BeginPlay()
{
    Super::BeginPlay();
    FVector NewLocation = GetActorLocation();
    NewLocation.Z += 30.0f;
    SetActorLocation(NewLocation);

    ApplyOutlineMaterial();
}

void AShield::ApplyOutlineMaterial()
{
    if (!ShieldMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("ShieldMesh가 없음!"));
        return;
    }

    if (OutlineMaterial)
    {
        UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
        if (DynamicMat)
        {
            ShieldMesh->SetOverlayMaterial(DynamicMat);
            
        }
    }
}

