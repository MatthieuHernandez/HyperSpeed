#include "CustomText.h"

#include "Components/TextBlock.h"
#include "Engine/UserInterfaceSettings.h"

void UCustomText::SetText(const FText& InText)
{
    Text = InText;
    if (TextBlock)
    {
        TextBlock->SetText(Text);
    }
}

void UCustomText::SetFontSize(float Size)
{
    FontSize = FMath::Max(1.0f, Size);
    if (TextBlock)
    {
        FSlateFontInfo Font = TextBlock->GetFont();
        // Match the point size entered in the standard UMG font details panel.
        Font.Size = GetDefault<UUserInterfaceSettings>()->ConvertFontSizeFromDisplayToNative(FontSize);
        TextBlock->SetFont(Font);
    }
}

void UCustomText::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    SetFontSize(FontSize);

    if (TextBlock)
    {
        TextBlock->SetText(Text);
    }
}
