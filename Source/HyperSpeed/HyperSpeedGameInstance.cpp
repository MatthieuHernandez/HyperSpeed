#include "HyperSpeedGameInstance.h"

#include "HyperSpeedSaveGame.h"
#include "MusicEngine.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"

void UHyperSpeedGameInstance::Init()
{
    Super::Init();

    if (GEngine)
    {
        if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
        {
            UserSettings->ApplySettings(false);
        }
    }

    Settings = Cast<UHyperSpeedSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Settings"), 0));
    if (!Settings)
    {
        Settings = Cast<UHyperSpeedSaveGame>(
            UGameplayStatics::CreateSaveGameObject(UHyperSpeedSaveGame::StaticClass()));
        if (Settings && !UGameplayStatics::SaveGameToSlot(Settings, TEXT("Settings"), 0))
        {
            UE_LOG(LogTemp, Warning, TEXT("Unable to save initial settings."));
        }
    }
}

void UHyperSpeedGameInstance::OnStart()
{
    Super::OnStart();

    if (UMusicEngine* MusicEngine = GetSubsystem<UMusicEngine>())
    {
        if (Settings)
        {
            MusicEngine->SetMasterVolume(Settings->MusicVolume);
        }
    }

    if (AudioProcessHandle.IsValid())
    {
        return;
    }

#if PLATFORM_LINUX
    const TCHAR* ExecutableName = TEXT("strudel-autoplay-linux");
#elif PLATFORM_WINDOWS
    const TCHAR* ExecutableName = TEXT("strudel-autoplay.exe");
#else
    UE_LOG(LogTemp, Warning, TEXT("Strudel autoplay is only configured for Windows and Linux."));
    return;
#endif

#if PLATFORM_WINDOWS || PLATFORM_LINUX
    const FString ExePath = FPaths::ConvertRelativePathToFull(
        FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/strudel-autoplay"), ExecutableName));
    if (!FPaths::FileExists(ExePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Strudel executable not found: %s"), *ExePath);
        return;
    }

    const FString WorkingDirectory = FPaths::GetPath(ExePath);
    AudioProcessHandle = FPlatformProcess::CreateProc(
        *ExePath, TEXT(""), false, true, true, nullptr, 0, *WorkingDirectory, nullptr);
    if (!AudioProcessHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Unable to start Strudel: %s"), *ExePath);
    }
#endif
}

void UHyperSpeedGameInstance::Shutdown()
{
    if (AudioProcessHandle.IsValid())
    {
        FPlatformProcess::TerminateProc(AudioProcessHandle, true);
        FPlatformProcess::CloseProc(AudioProcessHandle);
        AudioProcessHandle.Reset();
    }
    Super::Shutdown();
}
