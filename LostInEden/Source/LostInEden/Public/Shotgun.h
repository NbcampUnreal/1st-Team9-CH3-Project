#pragma once

#include "CoreMinimal.h"
#include "Gun.h"  // ✅ AGun을 상속받기 위해 추가
#include "Shotgun.generated.h"

UCLASS()
class AShotgun : public AGun  // ✅ AActor → AGun으로 변경
{
    GENERATED_BODY()

public:
    AShotgun();

protected:
    virtual void BeginPlay() override;

    int32 PelletCount;   // ✅ 한 번 발사될 탄환 개수
    float PelletSpread;  // ✅ 탄환 퍼짐 정도
public:
    void Fire() override;  // ✅ AGun의 Fire()를 오버라이딩
    void Reload() override; // ✅ AGun의 Reload()를 오버라이딩
    void FinishReload();
    void ResetFire(); // ✅ 발사 후 대기 시간 후 재사용 가능

private:
    UPROPERTY(EditAnywhere, Category = "Weapon Stats")
    float ReloadTime; // ✅ 재장전 시간

    UPROPERTY(EditAnywhere, Category = "Weapon Stats")
    bool bCanFire; // ✅ 연속 발사 방지용 변수

    FTimerHandle FireDelayTimer; // ✅ 발사 후 대기 시간 타이머
    FTimerHandle ReloadTimer; // ✅ 재장전 타이머
};