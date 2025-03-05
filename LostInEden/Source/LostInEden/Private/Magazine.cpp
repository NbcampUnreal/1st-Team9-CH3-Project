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

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> PistolMat(TEXT("/Game/Items/Material/PistolMagazine_Texture.PistolMagazine_Texture"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> RifleMat(TEXT("/Game/Items/Material/RifleMagazine_Material.RifleMagazine_Material"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> ShotgunMat(TEXT("/Game/Items/Material/ShotgunMagazine_Material.ShotgunMagazine_Material"));

    if (PistolMat.Succeeded()) PistolOutlineMaterial = PistolMat.Object;
    if (RifleMat.Succeeded()) RifleOutlineMaterial = RifleMat.Object;
    if (ShotgunMat.Succeeded()) ShotgunOutlineMaterial = ShotgunMat.Object;

}

void AMagazine::BeginPlay()
{
    Super::BeginPlay();
    ApplyOutlineMaterial();
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

int32 AMagazine::GetAmmoAmount() const
{
    return AmmoAmount;
}


void AMagazine::ApplyOutlineMaterial()
{
    if (!MagazineMesh) return;

    UMaterialInterface* SelectedMaterial = nullptr;

    switch (AmmoType)
    {
    case EItemType::PISTOL_BULLET:
        SelectedMaterial = PistolOutlineMaterial;
        break;
    case EItemType::RIFLE_BULLET:
        SelectedMaterial = RifleOutlineMaterial;
        break;
    case EItemType::SHOTGUN_BULLET:
        SelectedMaterial = ShotgunOutlineMaterial;
        break;
    default:
        SelectedMaterial = nullptr;
        break;
    }

    if (SelectedMaterial)
    {
        
        UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(SelectedMaterial, this);
        if (DynamicMat)
        {
            MagazineMesh->SetOverlayMaterial(DynamicMat);  
        }
    }

    UMaterialInterface* OriginalMaterial = MagazineMesh->GetMaterial(0);
    if (OriginalMaterial)
    {
        MagazineMesh->SetMaterial(0, OriginalMaterial);
    }
}
