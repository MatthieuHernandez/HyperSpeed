#pragma once
#include "CoreMinimal.h"
#include <array>
#include "Blueprint/UserWidget.h"
#include "SettingsMenu.generated.h"

class USoundClass;
class USoundBase;
class UCustomButton;
class UTextBlock;
class USlider;


UCLASS(abstract)
class HYPERSPEED_API USettingsMenu : public UUserWidget
{
    GENERATED_BODY()
protected:

    virtual void NativeConstruct() override;

    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> PreviousMenu;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundClass* MasterSoundClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    TObjectPtr<USoundBase> TestSound;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UCustomButton> RightScreenModeButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* ScreenModeText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UCustomButton> LeftScreenModeButton;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UCustomButton> RightResolutionButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* ResolutionText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UCustomButton> LeftResolutionButton;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UCustomButton> RightVsyncButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* VsyncText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UCustomButton> LeftVsyncButton;

    UPROPERTY(meta = (BindWidgetOptional))
    USlider* MusicVolumeSlider;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* MusicVolumeText;

    UPROPERTY(meta = (BindWidgetOptional))
    USlider* SoundEffectsVolumeSlider;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* SoundEffectsVolumeText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UCustomButton> BackButton;

private:

    UFUNCTION()
    void OnLeftScreenModeClicked();

    UFUNCTION()
    void OnRightScreenModeClicked();

    UFUNCTION()
    void OnLeftResolutionClicked();

    UFUNCTION()
    void OnRightResolutionClicked();

    UFUNCTION()
    void OnLeftVsyncClicked();

    UFUNCTION()
    void OnRightVsyncClicked();

    UFUNCTION()
    void OnMusicVolumeSliderChanged(float value);

    UFUNCTION()
    void OnMusicVolumeSliderReleased();

    UFUNCTION()
    void OnSoundEffectsVolumeSliderChanged(float value);

    UFUNCTION()
    void OnSoundEffectsVolumeSliderReleased();

    UFUNCTION()
    void OnBackClicked();

    inline static std::array<FString, 3> ScreenModeOptions = {"Borderless", "Fullscreen", "windowed"};
    inline static std::array<FString, 4> ResolutionOptions = {"2560x1440", "1366x768", "1600x900", "1920x1080"};
    inline static std::array<FString, 2> VsyncOptions = {"On ", "Off"};

    void ChangeScreenMode(uint32 index);
    void ChangeResolution(uint32 index);
    void ChangeVsync(uint32 index);
    void ChangeMusicVolume(float value);
    void ChangeSoundEffectVolume(float value);

public:
    void InitializeSettings();
    void SetPreviousMenu(UUserWidget* Menu) { PreviousMenu = Menu; }
};

