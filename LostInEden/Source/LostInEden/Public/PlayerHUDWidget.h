// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINEDEN_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // UI 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(int32 Health, int32 MaxHealth);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateShield(int32 Shield, int32 MaxShield);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 Ammo, int32 MaxAmmo);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdatePotionCount(int32 PotionCount);

protected:
    // 위젯 초기화
    virtual void NativeConstruct() override;

private:
    // UI 요소 참조 (BindWidget으로 연결)
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ShieldBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HealthText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PotionText;
	
};
