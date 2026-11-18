#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "HyperSpeedSaveGame.generated.h"

UCLASS()
class HYPERSPEED_API UHyperSpeedSaveGame : public USaveGame
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MusicVolume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SoundEffectVolume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Display")
    int32 ScreenModeIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Display")
    int32 ResolutionIndex = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Display")
    int32 VsyncIndex = 0;
};
