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
    // UI ������Ʈ �Լ�
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(int32 Health, int32 MaxHealth);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateShield(int32 Shield, int32 MaxShield);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 Ammo, int32 MaxAmmo);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdatePotionCount(int32 PotionCount);

protected:
    // ���� �ʱ�ȭ
    virtual void NativeConstruct() override;

private:
    // UI ��� ���� (BindWidget���� ����)
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
