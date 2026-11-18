#include "SettingsMenu.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "HyperSpeedSaveGame.h"

#include "MusicEngine.h"
#include "CustomButton.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "HyperSpeedGameInstance.h"

void USettingsMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (LeftScreenModeButton)
    {
        LeftScreenModeButton->OnClicked.AddUniqueDynamic(this, &USettingsMenu::OnLeftScreenModeClicked);
    }
    if (RightScreenModeButton)
    {
        RightScreenModeButton->OnClicked.AddUniqueDynamic(this, &USettingsMenu::OnRightScreenModeClicked);
    }
    if (LeftResolutionButton)
    {
        LeftResolutionButton->OnClicked.AddUniqueDynamic(this, &USettingsMenu::OnLeftResolutionClicked);
    }
    if (RightResolutionButton)
    {
        RightResolutionButton->OnClicked.AddUniqueDynamic(this, &USettingsMenu::OnRightResolutionClicked);
    }
    if (LeftVsyncButton)
    {
        LeftVsyncButton->OnClicked.AddUniqueDynamic(this, &USettingsMenu::OnLeftVsyncClicked);
    }
    if (RightVsyncButton)
    {
        RightVsyncButton->OnClicked.AddUniqueDynamic(this, &USettingsMenu::OnRightVsyncClicked);
    }
    if (BackButton)
    {
        BackButton->OnClicked.AddUniqueDynamic(this, &USettingsMenu::OnBackClicked);
    }
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &USettingsMenu::OnMusicVolumeSliderChanged);
        MusicVolumeSlider->OnMouseCaptureEnd.AddUniqueDynamic(this, &USettingsMenu::OnMusicVolumeSliderReleased);
        MusicVolumeSlider->OnControllerCaptureEnd.AddUniqueDynamic(this, &USettingsMenu::OnMusicVolumeSliderReleased);
    }
    if (SoundEffectsVolumeSlider)
    {
        SoundEffectsVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &USettingsMenu::OnSoundEffectsVolumeSliderChanged);
        SoundEffectsVolumeSlider->OnMouseCaptureEnd.AddUniqueDynamic(this, &USettingsMenu::OnSoundEffectsVolumeSliderReleased);
        SoundEffectsVolumeSlider->OnControllerCaptureEnd.AddUniqueDynamic(this, &USettingsMenu::OnSoundEffectsVolumeSliderReleased);
    }
    InitializeSettings();
}

void USettingsMenu::InitializeSettings()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->ScreenModeIndex = FMath::Clamp(gameInstance->Settings->ScreenModeIndex, 0, 2);
        gameInstance->Settings->ResolutionIndex = FMath::Clamp(gameInstance->Settings->ResolutionIndex, 0, 3);
        gameInstance->Settings->VsyncIndex = FMath::Clamp(gameInstance->Settings->VsyncIndex, 0, 1);
        ChangeScreenMode(gameInstance->Settings->ScreenModeIndex);
        ChangeResolution(gameInstance->Settings->ResolutionIndex);
        ChangeVsync(gameInstance->Settings->VsyncIndex);
        ChangeMusicVolume(gameInstance->Settings->MusicVolume);
        ChangeSoundEffectVolume(gameInstance->Settings->SoundEffectVolume);
    }
}

void USettingsMenu::ChangeMusicVolume(float value)
{
    value = FMath::Clamp(value, 0.0f, 1.0f);
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->MusicVolume = value;
        if (auto* MusicEngine = gameInstance->GetSubsystem<UMusicEngine>())
        {
            MusicEngine->SetMasterVolume(value);
        }
    }
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->SetValue(value);
    }
    if (MusicVolumeText)
    {
        const auto text = FText::Format(FText::FromString(TEXT("{0}%")), FText::AsNumber(FMath::RoundToInt(value * 100.0f)));
        MusicVolumeText->SetText(text);
    }
}

void USettingsMenu::ChangeSoundEffectVolume(float value)
{
    value = FMath::Clamp(value, 0.0f, 1.0f);
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->SoundEffectVolume = value;
    }
    if (MasterSoundClass)
    {
        MasterSoundClass->Properties.Volume = value;
    }
    if (SoundEffectsVolumeSlider)
    {
        SoundEffectsVolumeSlider->SetValue(value);
    }
    if (SoundEffectsVolumeText)
    {
        const auto text = FText::Format(FText::FromString(TEXT("{0}%")), FText::AsNumber(FMath::RoundToInt(value * 100.0f)));
        SoundEffectsVolumeText->SetText(text);
    }
}

void USettingsMenu::ChangeScreenMode(uint32 index)
{
    if (auto* settings = GEngine->GetGameUserSettings())
    {
        EWindowMode::Type WindowMode = EWindowMode::WindowedFullscreen;
        switch (index)
        {
        case 0:
            WindowMode = EWindowMode::WindowedFullscreen;
            break;
        case 1:
            WindowMode = EWindowMode::Fullscreen;
            break;
        case 2:
            WindowMode = EWindowMode::Windowed;
            break;
        default:
            break;
        }
        settings->SetFullscreenMode(WindowMode);
        settings->ApplySettings(false);
        settings->SaveSettings();
        if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
        {
            UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
            if (ScreenModeText)
            {
                ScreenModeText->SetText(FText::FromString(ScreenModeOptions[gameInstance->Settings->ScreenModeIndex]));
            }
        }
    }
}

void USettingsMenu::OnLeftScreenModeClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->ScreenModeIndex = gameInstance->Settings->ScreenModeIndex == 0 ? ScreenModeOptions.size() - 1 : gameInstance->Settings->ScreenModeIndex - 1;
        ChangeScreenMode(gameInstance->Settings->ScreenModeIndex);
    }
}

void USettingsMenu::OnRightScreenModeClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->ScreenModeIndex = (gameInstance->Settings->ScreenModeIndex == ScreenModeOptions.size() - 1) ? 0 : gameInstance->Settings->ScreenModeIndex + 1;
        ChangeScreenMode(gameInstance->Settings->ScreenModeIndex);
    }
}

void USettingsMenu::ChangeResolution(uint32 index)
{
    if (auto* settings = GEngine->GetGameUserSettings())
    {
        FIntPoint Resolution;
        switch (index)
        {
        case 0:
            Resolution = FIntPoint(2560, 1440);
            break;
        case 1:
            Resolution = FIntPoint(1366, 768);
            break;
        case 2:
            Resolution = FIntPoint(1600, 900);
            break;
        case 3:
            Resolution = FIntPoint(1920, 1080);
            break;
        default:
            return;
        }
        settings->SetScreenResolution(Resolution);
        settings->ApplySettings(false);
        settings->SaveSettings();
        if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
        {
            UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
            if (ResolutionText)
            {
                ResolutionText->SetText(FText::FromString(ResolutionOptions[gameInstance->Settings->ResolutionIndex]));
            }
        }
    }
}

void USettingsMenu::OnLeftResolutionClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->ResolutionIndex = gameInstance->Settings->ResolutionIndex == 0 ? ResolutionOptions.size() - 1 : gameInstance->Settings->ResolutionIndex - 1;
        ChangeResolution(gameInstance->Settings->ResolutionIndex);
    }
}

void USettingsMenu::OnRightResolutionClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->ResolutionIndex = gameInstance->Settings->ResolutionIndex == (ResolutionOptions.size() - 1) ? 0 : gameInstance->Settings->ResolutionIndex + 1;
        ChangeResolution(gameInstance->Settings->ResolutionIndex);
    }
}

void USettingsMenu::ChangeVsync(uint32 index)
{
    const bool value = (index == 0);
    if (auto* settings = GEngine->GetGameUserSettings())
    {
        settings->SetVSyncEnabled(value);
        settings->ApplySettings(false);
        settings->SaveSettings();
        if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
        {
            UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
            if (VsyncText)
            {
                VsyncText->SetText(FText::FromString(VsyncOptions[gameInstance->Settings->VsyncIndex]));
            }
        }
    }
}

void USettingsMenu::OnLeftVsyncClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->VsyncIndex = gameInstance->Settings->VsyncIndex == 0 ? VsyncOptions.size() - 1 : gameInstance->Settings->VsyncIndex - 1;
        ChangeVsync(gameInstance->Settings->VsyncIndex);
    }
}

void USettingsMenu::OnRightVsyncClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        gameInstance->Settings->VsyncIndex = (gameInstance->Settings->VsyncIndex == VsyncOptions.size() - 1) ? 0 : gameInstance->Settings->VsyncIndex + 1;
        ChangeVsync(gameInstance->Settings->VsyncIndex);
    }
}

void USettingsMenu::OnMusicVolumeSliderChanged(float value)
{
    ChangeMusicVolume(value);
}

void USettingsMenu::OnMusicVolumeSliderReleased()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
    }
}

void USettingsMenu::OnSoundEffectsVolumeSliderChanged(float value)
{
    ChangeSoundEffectVolume(value);
}

void USettingsMenu::OnSoundEffectsVolumeSliderReleased()
{
    if (auto* gameInstance = GetGameInstance<UHyperSpeedGameInstance>(); gameInstance && gameInstance->Settings)
    {
        UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
    }
    if (TestSound)
    {
        UGameplayStatics::PlaySound2D(this, TestSound);
    }
}

void USettingsMenu::OnBackClicked()
{
    auto* PC = GetOwningPlayer();
    if (!PC || !IsValid(PreviousMenu))
    {
        return;
    }

    if (auto* Instance = GetGameInstance<UHyperSpeedGameInstance>())
    {
        if (Instance->Settings)
        {
            UGameplayStatics::SaveGameToSlot(Instance->Settings, TEXT("Settings"), 0);
        }
    }
    PreviousMenu->AddToViewport();
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(PreviousMenu->TakeWidget());
    PC->SetInputMode(InputMode);
    PC->SetShowMouseCursor(true);
    RemoveFromParent();
}

