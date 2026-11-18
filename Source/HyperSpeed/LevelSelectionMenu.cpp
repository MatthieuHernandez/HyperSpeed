#include "LevelSelectionMenu.h"

#include "GameFramework/PlayerController.h"

#include "CustomButton.h"
#include "LevelDefinition.h"
#include "HyperSpeedGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULevelSelectionMenu::SetPreviousMenu(UUserWidget* Menu) { PreviousMenu = Menu; }

void ULevelSelectionMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (Map1Button)
    {
        Map1Button->OnClicked.AddDynamic(this, &ULevelSelectionMenu::OnMap1Clicked);
    }
    if (Map2Button)
    {
        Map2Button->OnClicked.AddDynamic(this, &ULevelSelectionMenu::OnMap2Clicked);
    }

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ULevelSelectionMenu::OnBackClicked);
    }
}

void ULevelSelectionMenu::OnMap1Clicked() { LaunchLevel(Map1Level); }

void ULevelSelectionMenu::OnMap2Clicked() { LaunchLevel(Map2Level); }

void ULevelSelectionMenu::LaunchLevel(ULevelDefinition* Level)
{
    UHyperSpeedGameInstance* Instance = Cast<UHyperSpeedGameInstance>(GetGameInstance());
    APlayerController* PC = GetOwningPlayer();
    if (!Instance || !IsValid(Level) || !PC)
    {
        UE_LOG(LogTemp, Error, TEXT("LevelSelectionMenu: configure HyperSpeedGameInstance, owning player and level definitions."));
        return;
    }

    if (!Level->LayoutTexture || Level->MusicFile.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("LevelSelectionMenu: %s requires a layout texture and music file."), *Level->GetName());
        return;
    }

    Instance->SelectedLevel = Level;
    PC->SetInputMode(FInputModeGameOnly());
    PC->SetShowMouseCursor(false);
    UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/L_Main")));
}

void ULevelSelectionMenu::OnBackClicked()
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC || !IsValid(PreviousMenu))
    {
        return;
    }

    PreviousMenu->AddToViewport();
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(PreviousMenu->TakeWidget());
    PC->SetInputMode(InputMode);
    PC->SetShowMouseCursor(true);
    RemoveFromParent();
}
