#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "LevelDefinition.generated.h"

class UTexture2D;

UCLASS(BlueprintType)
class HYPERSPEED_API ULevelDefinition : public UDataAsset
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    TObjectPtr<UTexture2D> LayoutTexture;

    // File name relative to Content/strudel-autoplay, e.g. KotoOverdrive.strudel.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    FString MusicFile;
};
