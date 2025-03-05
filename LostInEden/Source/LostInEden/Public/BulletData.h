#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BulletData.generated.h"

// 데이터 테이블에서 사용할 구조체 정의
USTRUCT(BlueprintType)
struct FBulletData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 탄환 블루프린트 경로
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
    TSoftClassPtr<AActor> BulletBlueprint;

    // 탄환 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
    float BulletSpeed = 5000.0f;

    // 탄환 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
    float Damage = 30.0f;
};
