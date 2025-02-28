// PlayerHUDWidget.cpp
#include "PlayerHUDWidget.h"
#include "Components/ProgressBar.h" // ProgressBar 헤더 포함
#include "Components/TextBlock.h"   // TextBlock 헤더 포함

void UPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 초기화 로직 (필요한 경우)
}

void UPlayerHUDWidget::UpdateHealth(int32 Health, int32 MaxHealth)
{
    if (HealthBar && HealthText)
    {
        // 체력 Progress Bar 업데이트
        float HealthPercent = static_cast<float>(Health) / static_cast<float>(MaxHealth);
        HealthBar->SetPercent(HealthPercent);

        // 체력 Text Block 업데이트
        FString HealthString = FString::Printf(TEXT("%d / %d"), Health, MaxHealth);
        HealthText->SetText(FText::FromString(HealthString));
    }
}

void UPlayerHUDWidget::UpdateShield(int32 Shield, int32 MaxShield)
{
    if (ShieldBar)
    {
        // 쉴드 Progress Bar 업데이트
        float ShieldPercent = static_cast<float>(Shield) / static_cast<float>(MaxShield);
        ShieldBar->SetPercent(ShieldPercent);
    }
}