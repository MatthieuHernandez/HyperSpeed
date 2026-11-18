#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainMenu.generated.h"

class UCustomButton;
class ULevelSelectionMenu;
class USettingsMenu;

UCLASS(Abstract)
class HYPERSPEED_API UMainMenu : public UUserWidget
{
    GENERATED_BODY()

  protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCustomButton> PlayButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCustomButton> QuitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCustomButton> SettingsButton;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<ULevelSelectionMenu> LevelSelectionMenuClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<USettingsMenu> SettingsMenuClass;

  private:
    UFUNCTION()
    void OnPlayClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnQuitClicked();
};
