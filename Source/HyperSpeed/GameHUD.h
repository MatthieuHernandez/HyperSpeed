#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "GameHUD.generated.h"

class UCustomText;

UCLASS(Abstract)
class HYPERSPEED_API UGameHUD : public UUserWidget
{
    GENERATED_BODY()

  protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
    TObjectPtr<UCustomText> TempoText;

    UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
    TObjectPtr<UCustomText> SpeedText;
};
