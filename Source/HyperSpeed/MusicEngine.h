#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "MusicEngine.generated.h"

UCLASS()
class HYPERSPEED_API UMusicEngine : public UGameInstanceSubsystem
{
    GENERATED_BODY()

  private:
    FString liveFile;
    FString variablesFile;

    bool load(FString& fileContent, const FString& file);
    bool save(const FString& fileContent, const FString& file);
    void writeLive(const FString& musicFile);
    void UpdateCurrentBPM(const FString& Content);
    float CurrentBPM = 0.0f;
    void SetMasterVolume(float value, const FString& file);
    void SetGlobalTempo(int32 value, const FString& file);

  public:
    UFUNCTION(BlueprintPure, Category = "Music")
    float GetCurrentBPM() const { return CurrentBPM; }

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    void SetMasterVolume(float value);
    void SetGlobalTempo(int32 value);

    void SetTempoFromSpeed(float SpeedMetersPerSecond);

    UFUNCTION(BlueprintCallable, Category = "Music")
    void PlayMusic(const FString& MusicFile);

    void PlayMenuMusic();
    void PlayDesertMusic();
};
