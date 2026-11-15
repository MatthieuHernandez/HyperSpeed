#include "CustomButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCustomButton::SetButtonText(const FText& Text)
{
    if (CustomText)
    {
        CustomText->SetText(Text);
    }
}

void UCustomButton::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (CustomButton)
    {
        CustomButton->OnClicked.AddDynamic(this, &UCustomButton::HandleClicked);
        CustomButton->OnPressed.AddDynamic(this, &UCustomButton::HandlePressed);
        CustomButton->OnReleased.AddDynamic(this, &UCustomButton::HandleReleased);
    }
}

void UCustomButton::HandleClicked() { OnClicked.Broadcast(); }

void UCustomButton::HandlePressed() { OnPressed.Broadcast(); }

void UCustomButton::HandleReleased() { OnReleased.Broadcast(); }
