#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ShipPawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UGameHUD;
struct FInputActionValue;

UCLASS(Abstract)
class HYPERSPEED_API AShipPawn : public APawn
{
    GENERATED_BODY()

  public:
    AShipPawn();

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetForwardSpeedKmh() const { return ForwardSpeedKmh; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
    float InitialSpeed = 3000.0f;

    // Constant lateral speed towards Y=-200, 0 or +200, including the return to center.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
    float StrafeSpeed = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> ShipMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> StrafeAction;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UGameHUD> GameHUDClass;

  private:
    void ShowGameHUD();

    UPROPERTY(Transient)
    TObjectPtr<UGameHUD> GameHUD;

    void HandleStrafe(const FInputActionValue& Value);
    void StopStrafe(const FInputActionValue& Value);

    float StrafeInput = 0.0f;
    float ForwardSpeedKmh = 0.0f;
};
