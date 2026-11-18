#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "MenuGameMode.generated.h"

class UMainMenu;

UCLASS(Abstract)
class HYPERSPEED_API AMenuGameMode : public AGameMode
{
    GENERATED_BODY()

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMainMenu> MainMenuClass;

  private:
    UPROPERTY(Transient)
    TObjectPtr<UMainMenu> MainMenu;
};
