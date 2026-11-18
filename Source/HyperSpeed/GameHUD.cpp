#include "GameHUD.h"

#include "CustomText.h"
#include "MusicEngine.h"
#include "ShipPawn.h"
#include "Engine/GameInstance.h"

void UGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UGameInstance* Instance = GetGameInstance();
    UMusicEngine* Music = Instance ? Instance->GetSubsystem<UMusicEngine>() : nullptr;
    if (TempoText)
    {
        const float BPM = Music ? Music->GetCurrentBPM() : 0.0f;
        TempoText->SetText(BPM > 0.0f
            ? FText::Format(NSLOCTEXT("GameHUD", "Tempo", "{0} BPM"), FText::AsNumber(FMath::RoundToInt(BPM)))
            : NSLOCTEXT("GameHUD", "UnknownTempo", "-- BPM"));
    }

    if (SpeedText)
    {
        const AShipPawn* Ship = Cast<AShipPawn>(GetOwningPlayerPawn());
        const int32 Speed = Ship ? FMath::RoundToInt(Ship->GetForwardSpeedKmh()) : 0;
        SpeedText->SetText(FText::Format(NSLOCTEXT("GameHUD", "Speed", "{0} km/h"), FText::AsNumber(Speed)));
    }
}
