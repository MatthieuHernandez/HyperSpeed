#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HAL/PlatformProcess.h"

#include "HyperSpeedGameInstance.generated.h"

class ULevelDefinition;
class UHyperSpeedSaveGame;

UCLASS()
class HYPERSPEED_API UHyperSpeedGameInstance : public UGameInstance
{
    GENERATED_BODY()

  public:
    virtual void Init() override;
    virtual void OnStart() override;
    virtual void Shutdown() override;

    UPROPERTY(BlueprintReadOnly, Category = "Settings")
    TObjectPtr<UHyperSpeedSaveGame> Settings;

    UPROPERTY(BlueprintReadWrite, Category = "Level")
    TObjectPtr<ULevelDefinition> SelectedLevel;

  private:
    FProcHandle AudioProcessHandle;
};
