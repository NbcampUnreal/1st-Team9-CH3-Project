#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gun.h"
#include "Rifle.h" // 🔹 라이플 클래스 포함
#include "InputActionValue.h"
#include "TestCharacter.generated.h"

UCLASS()
class LOSTINEDEN_API ATestCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATestCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** 📌 카메라 관리 설정 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* CameraComp;

    /** 📌 무기 관리 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AGun* EquippedGun;

    /** 📌 무기 관리 */
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AGun> GunClass;  // 🔹 AGun을 기반으로 모든 무기 표시 가능


    /** 📌 이동 및 조준 관련 함수 */
    void Move(const FInputActionValue& InputValue);
    void Turn(const FInputActionValue& InputValue);
    void LookUp(const FInputActionValue& InputValue);
    void Fire();
    void Reload();
    void EquipGun();
    void StopFiring();


    /** 📌 입력 매핑 설정 */
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputMappingContext* imc_TPS;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* ia_Lookup;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* ia_Turn;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* ia_Move;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* ia_Fire;

    void EquipGun(bool bIsLeftHand); // 🔹 왼손(true) 또는 오른손(false) 장착 가능
};
