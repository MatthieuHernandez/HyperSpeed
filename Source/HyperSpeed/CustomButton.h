#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "CustomButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomButtonReleased);

UCLASS(Abstract)
class MAGICGOOSE_API UCustomButton : public UUserWidget
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "Button")
    void SetButtonText(const FText& Text);

    UPROPERTY(BlueprintAssignable, Category = "Button")
    FCustomButtonClicked OnClicked;

    UPROPERTY(BlueprintAssignable, Category = "Button")
    FCustomButtonPressed OnPressed;

    UPROPERTY(BlueprintAssignable, Category = "Button")
    FCustomButtonReleased OnReleased;

  protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> CustomButton;

    UPROPERTY(BlueprintReadOnly, Category = "Button", meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> CustomText;

  private:
    UFUNCTION()
    void HandleClicked();

    UFUNCTION()
    void HandlePressed();

    UFUNCTION()
    void HandleReleased();
};
