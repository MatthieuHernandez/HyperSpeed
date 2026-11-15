#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "MapLayout.generated.h"

class UTexture2D;

UCLASS()
class HYPERSPEED_API UMapLayout : public UObject
{
    GENERATED_BODY()

  public:
    bool ImportLayout(UTexture2D* Texture);

    int32 Width = 0;
    int32 Height = 0;
    TArray<FIntPoint> ObstaclePixels;
};
