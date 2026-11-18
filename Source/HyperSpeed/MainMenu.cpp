#include "MainMenu.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

#include "CustomButton.h"
#include "LevelSelectionMenu.h"
#include "SettingsMenu.h"

void UMainMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::OnSettingsClicked);
    }

    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMainMenu::OnPlayClicked);
    }
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClicked);
    }
}

void UMainMenu::OnPlayClicked()
{
    auto* PC = GetOwningPlayer();
    if (!PC || !LevelSelectionMenuClass)
    {
        return;
    }

    auto* Menu = CreateWidget<ULevelSelectionMenu>(PC, LevelSelectionMenuClass);
    if (!Menu)
    {
        return;
    }

    Menu->SetPreviousMenu(this);
    Menu->AddToViewport();
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(Menu->TakeWidget());
    PC->SetInputMode(InputMode);
    PC->SetShowMouseCursor(true);
    RemoveFromParent();
}

void UMainMenu::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMainMenu::OnSettingsClicked()
{
    auto* PC = GetOwningPlayer();
    if (!PC || !SettingsMenuClass)
    {
        return;
    }
    auto* Menu = CreateWidget<USettingsMenu>(PC, SettingsMenuClass);
    if (!Menu)
    {
        return;
    }
    Menu->SetPreviousMenu(this);
    Menu->SetIsFocusable(true);
    Menu->AddToViewport();
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(Menu->TakeWidget());
    PC->SetInputMode(InputMode);
    PC->SetShowMouseCursor(true);
    RemoveFromParent();
}
