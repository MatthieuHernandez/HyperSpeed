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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
    float MinSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
    float MaxSpeed = 6000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
    float Acceleration = 1000.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement", meta = (Units = "cm/s"))
    float CurrentSpeed = 0.0f;

    // Constant lateral speed towards Y=-200, 0 or +200, including the return to center.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
    float StrafeSpeed = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> ShipMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> StrafeAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> AccelerateAction;

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
    void HandleAcceleration(const FInputActionValue& Value);
    void StopAcceleration(const FInputActionValue& Value);

    float StrafeInput = 0.0f;
    float AccelerationInput = 0.0f;
    float ForwardSpeedKmh = 0.0f;
    float TempoUpdateElapsed = 0.0f;
};
