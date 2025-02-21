#include "Shotgun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "TestCharacter.h"

AShotgun::AShotgun()
{
    Damage = 10.0f;
    FireRate = 1.0f;
    MaxAmmo = 8;
    CurrentAmmo = MaxAmmo;
    Range = 1500.0f;

    PelletCount = 6; // 한 번에 발사될 탄환 수
    PelletSpread = 5.0f; // 탄환 퍼짐 정도
    ReloadTime = 2.5f;
}

void AShotgun::Fire()
{
    if (CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 탄약 없음! 현재 탄약: %d"), CurrentAmmo);
        return;
    }

    if (!BulletFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet Factory가 설정되지 않음! 블루프린트에서 확인하세요."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World가 없음!"));
        return;
    }

    if (!MuzzleLocation)
    {
        UE_LOG(LogTemp, Error, TEXT("MuzzleLocation이 설정되지 않음! 블루프린트에서 확인하세요."));
        return;
    }

    // 🔹 플레이어의 카메라 방향 가져오기 (보다 직관적인 조준)
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("소유 캐릭터를 찾을 수 없음!"));
        return;
    }

    // 🔹 플레이어 캐릭터인지 확인 후 캐스팅 (만약 AMyCharacter라면)
    ATestCharacter* PlayerCharacter = Cast<ATestCharacter>(OwnerCharacter);
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerCharacter로 캐스팅할 수 없음!"));
        return;
    }

    // 🔹 카메라 컴포넌트 찾기
    UCameraComponent* CameraComponent = PlayerCharacter->FindComponentByClass<UCameraComponent>();
    if (!CameraComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("카메라 컴포넌트를 찾을 수 없음!"));
        return;
    }


    // 🔹 Muzzle(총구) 위치 및 기본 방향
    FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
    FRotator MuzzleRot = MuzzleLocation->GetComponentRotation();
    FVector CameraDirection = CameraComponent->GetForwardVector();

    CurrentAmmo--;

    int32 SpawnedPellets = 0;
    for (int32 i = 0; i < PelletCount; i++)
    {
        // 🔹 Spread를 월드 회전이 아닌 MuzzleRot 기준으로 적용
        float SpreadYaw = FMath::RandRange(-PelletSpread, PelletSpread);
        float SpreadPitch = FMath::RandRange(-PelletSpread, PelletSpread);

        // 🔹 회전을 적용하여 방향 설정
        FRotator AdjustedRot = (CameraDirection * Range).Rotation();
        AdjustedRot.Yaw += SpreadYaw;
        AdjustedRot.Pitch += SpreadPitch;

        FVector ShotDirection = AdjustedRot.Vector();

        // 🔹 라인 트레이스 시작점과 끝점 설정
        FVector TraceStart = MuzzlePos;
        FVector TraceEnd = TraceStart + (ShotDirection * Range);

        // 🔹 라인 트레이스 실행
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);

        bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

        // 🔹 라인 트레이스 시각화 (빨간 선)
        DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 1.0f);

        // 🔹 맞았다면 탄환 방향 조정
        if (bHit)
        {
            ShotDirection = (HitResult.ImpactPoint - MuzzlePos).GetSafeNormal();
        }

        // 🔹 디버그 로그 출력
        UE_LOG(LogTemp, Warning, TEXT("Shot %d - AdjustedRot: %s | ShotDirection: %s"),
            i, *AdjustedRot.ToString(), *ShotDirection.ToString());

        // 🔹 총알 스폰
        ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletFactory, MuzzlePos, ShotDirection.Rotation());
        if (SpawnedBullet)
        {
            SpawnedPellets++;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("샷건 탄환 스폰 실패! BulletFactory 또는 MuzzlePos 확인 필요"));
        }
    }

    if (SpawnedPellets > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 탄환 %d개 스폰 성공! 남은 탄약: %d"), SpawnedPellets, CurrentAmmo);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("샷건 탄환이 하나도 생성되지 않음! 블루프린트 설정 확인 필요"));
    }
}

void AShotgun::Reload()
{
    if (CurrentAmmo >= MaxAmmo)
    {
        UE_LOG(LogTemp, Warning, TEXT("샷건 이미 탄창 가득 참!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("샷건 재장전! 탄약: %d -> %d"), CurrentAmmo, MaxAmmo);
    CurrentAmmo = MaxAmmo;
}

void AShotgun::BeginPlay()
{
    Super::BeginPlay();

    Reload(); // ✅ 게임 시작 시 탄창 채우기

    UE_LOG(LogTemp, Warning, TEXT("샷건 시작! 현재 탄약: %d"), CurrentAmmo);
}
