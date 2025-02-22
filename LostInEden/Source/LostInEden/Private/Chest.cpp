#include "Chest.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "Blueprint/UserWidget.h"

AChest::AChest()
{
    PrimaryActorTick.bCanEverTick = false;

    ChestMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestMesh"));
    RootComponent = ChestMesh;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AChest::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AChest::OnOverlapEnd);
}

void AChest::BeginPlay()
{
    Super::BeginPlay();
}

// ✅ 플레이어가 근처에 가면 "E 키를 눌러 상자를 여세요" 표시
void AChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
    if (Player)
    {
        bCanInteract = true;
        Player->ShowInteractMessage(true); // ✅ E 키를 누르라고 메시지 표시
    }
}

// ✅ 플레이어가 떠나면 메시지 숨김
void AChest::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
    if (Player)
    {
        bCanInteract = false;
        Player->ShowInteractMessage(false);
    }
}

// ✅ E 키를 눌렀을 때 실행 (상자 열기)
void AChest::OpenChest()
{
    if (bCanInteract)
    {
        UE_LOG(LogTemp, Log, TEXT("Chest Opened!"));

        // ✅ UI 열기 (블루프린트에서 구현 가능)
        OnChestOpened.Broadcast();

        // ✅ 애니메이션 실행
        if (OpenAnim)
        {
            ChestMesh->PlayAnimation(OpenAnim, false);
        }

        // ✅ 소리 실행
        if (OpenSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
        }
    }
}

// ✅ UI 닫기 (상자 닫기)
void AChest::CloseChest()
{
    UE_LOG(LogTemp, Log, TEXT("Chest Closed!"));

    // ✅ UI 닫기 (블루프린트에서 구현 가능)

    // ✅ 애니메이션 실행
    if (CloseAnim)
    {
        ChestMesh->PlayAnimation(CloseAnim, false);
    }
}
