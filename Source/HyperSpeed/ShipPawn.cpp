#include "ShipPawn.h"
#include "GameHUD.h"
#include "MusicEngine.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AShipPawn::AShipPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<UGameHUD> HUDAsset(TEXT("/Game/Menus/WBP_GameHUD"));
    GameHUDClass = HUDAsset.Class;

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextAsset(
        TEXT("/Game/Player/IMC_Ship.IMC_Ship"));
    ShipMappingContext = MappingContextAsset.Object;

    static ConstructorHelpers::FObjectFinder<UInputAction> StrafeActionAsset(
        TEXT("/Game/Player/IA_Strafe.IA_Strafe"));
    StrafeAction = StrafeActionAsset.Object;
}

// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
    const float LowerSpeed = FMath::Max(0.0f, MinSpeed);
    CurrentSpeed = FMath::Clamp(InitialSpeed, LowerSpeed, FMath::Max(LowerSpeed, MaxSpeed));
}

// Called every frame
void AShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // The existing mapping gives Q=-1 and D=+1; Q targets the right lane (+Y).
    const double TargetY = -FMath::Sign(StrafeInput) * 200.0;
    const double CurrentY = GetActorLocation().Y;
    const double NextY = FMath::FInterpConstantTo(CurrentY, TargetY, DeltaTime, StrafeSpeed);

    const float LowerSpeed = FMath::Max(0.0f, MinSpeed);
    CurrentSpeed = FMath::Clamp(
        CurrentSpeed + AccelerationInput * FMath::Max(0.0f, Acceleration) * DeltaTime,
        LowerSpeed, FMath::Max(LowerSpeed, MaxSpeed));

    FVector Movement = GetActorForwardVector() * CurrentSpeed * DeltaTime;
    Movement.Y = NextY - CurrentY;
    const FVector PreviousLocation = GetActorLocation();
    AddActorWorldOffset(Movement, true);
    ForwardSpeedKmh = DeltaTime > SMALL_NUMBER
        ? FMath::Abs(GetActorLocation().X - PreviousLocation.X) / DeltaTime * 0.036f
        : 0.0f;

    TempoUpdateElapsed += DeltaTime;
    if (TempoUpdateElapsed >= 0.1f && IsLocallyControlled())
    {
        TempoUpdateElapsed = 0.0f;
        if (UGameInstance* Instance = GetGameInstance())
        {
            if (UMusicEngine* Music = Instance->GetSubsystem<UMusicEngine>())
            {
                Music->SetTempoFromSpeed(ForwardSpeedKmh / 3.6f);
            }
        }
    }
}

// Called to bind functionality to input
void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    StrafeInput = 0.0f;
    AccelerationInput = 0.0f;

    ShowGameHUD();

    if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (ShipMappingContext)
                {
                    Subsystem->AddMappingContext(ShipMappingContext, 0);
                }
            }
        }
    }

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (AccelerateAction)
        {
            EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &AShipPawn::HandleAcceleration);
            EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Completed, this, &AShipPawn::StopAcceleration);
            EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Canceled, this, &AShipPawn::StopAcceleration);
        }
        if (StrafeAction)
        {
            EnhancedInput->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &AShipPawn::HandleStrafe);
            EnhancedInput->BindAction(StrafeAction, ETriggerEvent::Completed, this, &AShipPawn::StopStrafe);
            EnhancedInput->BindAction(StrafeAction, ETriggerEvent::Canceled, this, &AShipPawn::StopStrafe);
        }
    }
}

void AShipPawn::HandleStrafe(const FInputActionValue& Value)
{
    StrafeInput = FMath::Clamp(Value.Get<float>(), -1.0f, 1.0f);
}

void AShipPawn::StopStrafe(const FInputActionValue& Value)
{
    StrafeInput = 0.0f;
}

void AShipPawn::HandleAcceleration(const FInputActionValue& Value)
{
    AccelerationInput = FMath::Clamp(Value.Get<float>(), -1.0f, 1.0f);
}

void AShipPawn::StopAcceleration(const FInputActionValue& Value)
{
    AccelerationInput = 0.0f;
}

void AShipPawn::ShowGameHUD()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC || !PC->IsLocalController() || GameHUD ||
        UGameplayStatics::GetCurrentLevelName(this, true) != TEXT("L_Main"))
    {
        return;
    }
    if (!GameHUDClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ShipPawn: GameHUDClass is not configured."));
        return;
    }

    GameHUD = CreateWidget<UGameHUD>(PC, GameHUDClass);
    if (GameHUD)
    {
        GameHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
        GameHUD->AddToPlayerScreen();
        PC->SetInputMode(FInputModeGameOnly());
        PC->SetShowMouseCursor(false);
    }
}

void AShipPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GameHUD)
    {
        GameHUD->RemoveFromParent();
        GameHUD = nullptr;
    }
    Super::EndPlay(EndPlayReason);
}
