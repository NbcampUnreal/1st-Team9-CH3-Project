// PlayerHUDWidget.cpp
#include "PlayerHUDWidget.h"
#include "Components/ProgressBar.h" // ProgressBar ��� ����
#include "Components/TextBlock.h"   // TextBlock ��� ����

void UPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // �ʱ�ȭ ���� (�ʿ��� ���)
}

void UPlayerHUDWidget::UpdateHealth(int32 Health, int32 MaxHealth)
{
    if (HealthBar && HealthText)
    {
        // ü�� Progress Bar ������Ʈ
        float HealthPercent = static_cast<float>(Health) / static_cast<float>(MaxHealth);
        HealthBar->SetPercent(HealthPercent);

        // ü�� Text Block ������Ʈ
        FString HealthString = FString::Printf(TEXT("%d / %d"), Health, MaxHealth);
        HealthText->SetText(FText::FromString(HealthString));
    }
}

void UPlayerHUDWidget::UpdateShield(int32 Shield, int32 MaxShield)
{
    if (ShieldBar)
    {
        // ���� Progress Bar ������Ʈ
        float ShieldPercent = static_cast<float>(Shield) / static_cast<float>(MaxShield);
        ShieldBar->SetPercent(ShieldPercent);
    }
}