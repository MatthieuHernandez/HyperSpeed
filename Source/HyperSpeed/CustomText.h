#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "CustomText.generated.h"

class UTextBlock;

UCLASS(Abstract)
class HYPERSPEED_API UCustomText : public UUserWidget
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "Text")
    void SetText(const FText& InText);

    UFUNCTION(BlueprintCallable, Category = "Text")
    void SetFontSize(float Size);

  protected:
    virtual void SynchronizeProperties() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text", meta = (ExposeOnSpawn = "true"))
    FText Text;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text", meta = (ClampMin = "1", ExposeOnSpawn = "true"))
    float FontSize = 32.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Text", meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock;
};
