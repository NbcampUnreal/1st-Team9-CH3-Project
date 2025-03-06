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
        FString HealthString = FString::Printf(TEXT("%d"), Health);
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

void UPlayerHUDWidget::UpdatePotionCount(int32 PotionCount)
{
    if (PotionText)
    {
        FString PotionString = FString::Printf(TEXT("%d"), PotionCount);
        PotionText->SetText(FText::FromString(PotionString));
    }
}

void UPlayerHUDWidget::UpdateAmmo(int32 Ammo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        FString AmmoString;

        if (Ammo == 500)
        {
            AmmoString = FString::Printf(TEXT("MAX"));  // ���� ź�� ǥ��
        }
        else
        {
            AmmoString = FString::Printf(TEXT("%d / %d"), Ammo, MaxAmmo);
        }

        AmmoText->SetText(FText::FromString(AmmoString));
    }
}
