#include "Chest.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Character.h"  // ✅ 플레이어 확인을 위해 추가

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

void AChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // ✅ 플레이어인지 확인
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
    {
        bPlayerInTrigger = true;  // ✅ 플레이어가 트리거 박스 안에 있음
        if (!bIsOpened && !bIsClosing)
        {
            OpenChest();
            GetWorldTimerManager().ClearTimer(CloseTimerHandle); // ✅ 닫기 타이머 취소
        }
    }
}

void AChest::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // ✅ 플레이어인지 확인
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
    {
        bPlayerInTrigger = false;  // ✅ 플레이어가 트리거 박스에서 나감
        if (bIsOpened)
        {
            bIsClosing = true;  // ✅ 닫히는 중 상태
            GetWorldTimerManager().SetTimer(CloseTimerHandle, this, &AChest::CloseChest, CloseDelay, false);
        }
    }
}

void AChest::OpenChest()
{
    bIsOpened = true;
    bIsClosing = false;  // ✅ 닫히는 중 상태 해제

    if (OpenAnim)
    {
        ChestMesh->PlayAnimation(OpenAnim, false);
    }

    if (OpenSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
    }

    UE_LOG(LogTemp, Log, TEXT("Chest Opened!"));
}

void AChest::CloseChest()
{
    // ✅ 플레이어가 아직 트리거 박스 안에 있으면 닫지 않음
    if (bPlayerInTrigger)
    {
        bIsClosing = false; // 닫기 취소
        return;
    }

    bIsOpened = false;
    bIsClosing = false;  // ✅ 닫히는 중 상태 해제

    if (OpenAnim)
    {
        ChestMesh->PlayAnimation(OpenAnim, false);
        ChestMesh->SetPlayRate(-1.0f); // ✅ 역방향 재생
        ChestMesh->SetPosition(OpenAnim->GetMaxCurrentTime(), false); // ✅ 마지막 프레임에서 시작
    }

    if (CloseSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
    }

    UE_LOG(LogTemp, Log, TEXT("Chest Closed!"));
}
