#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "LevelSelectionMenu.generated.h"

class UCustomButton;
class ULevelDefinition;

UCLASS(Abstract)
class HYPERSPEED_API ULevelSelectionMenu : public UUserWidget
{
    GENERATED_BODY()

  public:
    void SetPreviousMenu(UUserWidget* Menu);

  protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels")
    TObjectPtr<ULevelDefinition> Map1Level;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels")
    TObjectPtr<ULevelDefinition> Map2Level;

    UPROPERTY(BlueprintReadOnly, Category = "Buttons", meta = (BindWidget))
    TObjectPtr<UCustomButton> Map1Button;

    UPROPERTY(BlueprintReadOnly, Category = "Buttons", meta = (BindWidget))
    TObjectPtr<UCustomButton> Map2Button;

    UPROPERTY(BlueprintReadOnly, Category = "Buttons", meta = (BindWidget))
    TObjectPtr<UCustomButton> BackButton;

  private:
    void LaunchLevel(ULevelDefinition* Level);

    UFUNCTION()
    void OnMap1Clicked();

    UFUNCTION()
    void OnMap2Clicked();

    UFUNCTION()
    void OnBackClicked();

    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> PreviousMenu;
};
