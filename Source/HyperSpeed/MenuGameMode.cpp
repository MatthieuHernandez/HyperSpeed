#include "MenuGameMode.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "MainMenu.h"
#include "MusicEngine.h"
#include "Engine/GameInstance.h"

void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UGameInstance* Instance = GetGameInstance())
    {
        if (UMusicEngine* Music = Instance->GetSubsystem<UMusicEngine>())
        {
            Music->PlayMenuMusic();
        }
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!MainMenuClass)
    {
        UE_LOG(LogTemp, Error, TEXT("MenuGameMode: assign Main Menu Class to WBP_MainMenu in Class Defaults."));
        return;
    }
    if (!PC || !PC->IsLocalController())
    {
        UE_LOG(LogTemp, Warning, TEXT("MenuGameMode: no local player controller; menu cannot be displayed."));
        return;
    }

    MainMenu = CreateWidget<UMainMenu>(PC, MainMenuClass);
    if (!MainMenu)
    {
        UE_LOG(LogTemp, Error, TEXT("MenuGameMode: failed to create %s."), *MainMenuClass->GetPathName());
        return;
    }

    MainMenu->AddToViewport();
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
    PC->SetInputMode(InputMode);
    PC->SetShowMouseCursor(true);
    UE_LOG(LogTemp, Display, TEXT("MenuGameMode: added %s to the viewport."), *MainMenu->GetName());
}
