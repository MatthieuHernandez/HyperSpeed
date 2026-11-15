#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RoadManager.generated.h"

class AShipPawn;
class UTexture2D;
class UMapLayout;

UCLASS(Abstract)
class HYPERSPEED_API ARoadManager : public AActor
{
    GENERATED_BODY()

  public:
    ARoadManager();

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Road")
    TArray<TObjectPtr<AActor>> RoadSegments;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Road")
    TObjectPtr<AShipPawn> Ship;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacles")
    TObjectPtr<UTexture2D> LayoutTexture;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacles")
    TSubclassOf<AActor> ObstacleClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacles", meta = (ClampMin = "1.0", Units = "cm"))
    float LayoutWidth = 600.0f;

    // Offset above the road's upper bounding surface, for the obstacle's pivot.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacles", meta = (Units = "cm"))
    float ObstacleHeightOffset = 0.0f;

    virtual void Tick(float DeltaTime) override;

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  private:
    void SpawnObstacles(AActor* Road);
    void RemovePassedObstacles();

    UPROPERTY(Transient)
    TObjectPtr<UMapLayout> Layout;

    UPROPERTY(Transient)
    TArray<TObjectPtr<AActor>> SpawnedObstacles;
};
