#include "RoadManager.h"

#include "Kismet/GameplayStatics.h"

#include "ShipPawn.h"
#include "MapLayout.h"
#include "HyperSpeedGameInstance.h"
#include "LevelDefinition.h"
#include "MusicEngine.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
constexpr double RoadLength = 10000.0;
constexpr double RecycleDistance = 50.0;
}  // namespace

ARoadManager::ARoadManager()
{
    PrimaryActorTick.bCanEverTick = true;
    static ConstructorHelpers::FClassFinder<AActor> ObstacleBP(TEXT("/Game/Environment/BP_Obstacle"));
    ObstacleClass = ObstacleBP.Class;
}

void ARoadManager::BeginPlay()
{
    Super::BeginPlay();

    if (RoadSegments.Num() != 3 || !IsValid(RoadSegments[0]) || !IsValid(RoadSegments[1]) ||
        !IsValid(RoadSegments[2]) || RoadSegments[0] == RoadSegments[1] || RoadSegments[0] == RoadSegments[2] ||
        RoadSegments[1] == RoadSegments[2])
    {
        UE_LOG(LogTemp, Warning, TEXT("RoadManager requires three distinct road actors."));
        SetActorTickEnabled(false);
        return;
    }

    RoadSegments.Sort([](const AActor& Left, const AActor& Right)
                      { return Left.GetActorLocation().X < Right.GetActorLocation().X; });

    if (!IsValid(Ship))
    {
        Ship = Cast<AShipPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    }
    if (const UHyperSpeedGameInstance* Instance = Cast<UHyperSpeedGameInstance>(GetGameInstance()))
    {
        if (IsValid(Instance->SelectedLevel))
        {
            LayoutTexture = Instance->SelectedLevel->LayoutTexture;
            if (UMusicEngine* MusicEngine = Instance->GetSubsystem<UMusicEngine>())
            {
                MusicEngine->PlayMusic(Instance->SelectedLevel->MusicFile);
            }
        }
    }
    if (LayoutTexture && ObstacleClass)
    {
        Layout = NewObject<UMapLayout>(this);
        if (Layout->ImportLayout(LayoutTexture))
        {
            for (AActor* Road : RoadSegments)
            {
                SpawnObstacles(Road);
            }
        }
    }
}

void ARoadManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!IsValid(Ship))
    {
        Ship = Cast<AShipPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    }

    if (!IsValid(Ship) || RoadSegments.Num() != 3 || !IsValid(RoadSegments[0]) || !IsValid(RoadSegments[1]) ||
        !IsValid(RoadSegments[2]))
    {
        return;
    }
    while (true)
    {
        FVector MiddleOrigin;
        FVector MiddleExtent;
        RoadSegments[1]->GetActorBounds(false, MiddleOrigin, MiddleExtent);
        const double MiddleStartX = MiddleOrigin.X - MiddleExtent.X;

        if (Ship->GetActorLocation().X < MiddleStartX + RecycleDistance)
        {
            break;
        }

        const TObjectPtr<AActor> PreviousRoad = RoadSegments[0];
        const FVector NextLocation = RoadSegments[2]->GetActorLocation() + FVector(RoadLength, 0.0, 0.0);
        if (!PreviousRoad->SetActorLocation(NextLocation, false, nullptr, ETeleportType::TeleportPhysics))
        {
            UE_LOG(LogTemp, Warning, TEXT("RoadManager could not move road actor %s."), *PreviousRoad->GetName());
            break;
        }

        RoadSegments.RemoveAt(0);
        RoadSegments.Add(PreviousRoad);
        SpawnObstacles(PreviousRoad);
    }
    RemovePassedObstacles();
}

void ARoadManager::SpawnObstacles(AActor* Road)
{
    if (!IsValid(Layout) || Layout->Width <= 0 || Layout->Height <= 0 || !ObstacleClass)
    {
        return;
    }

    FVector Origin;
    FVector Extent;
    Road->GetActorBounds(false, Origin, Extent);
    const double StartX = Origin.X - Extent.X;
    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    for (const FIntPoint& Pixel : Layout->ObstaclePixels)
    {
        // Pixel centers: columns span Y, rows run from the start to the end of the road.
        const FVector Location(
            StartX + (Pixel.Y + 0.5) * RoadLength / Layout->Height,
            Origin.Y + ((Pixel.X + 0.5) / Layout->Width - 0.5) * LayoutWidth,
            Origin.Z + Extent.Z + ObstacleHeightOffset);
        if (IsValid(Ship) && Location.X < Ship->GetActorLocation().X - RecycleDistance)
        {
            continue;
        }
        if (AActor* Obstacle = GetWorld()->SpawnActor<AActor>(ObstacleClass, Location, FRotator::ZeroRotator, Params))
        {
            SpawnedObstacles.Add(Obstacle);
        }
    }
}

void ARoadManager::RemovePassedObstacles()
{
    const double DeleteBeforeX = Ship->GetActorLocation().X - RecycleDistance;
    for (int32 Index = SpawnedObstacles.Num() - 1; Index >= 0; --Index)
    {
        AActor* Obstacle = SpawnedObstacles[Index];
        if (!IsValid(Obstacle) || Obstacle->GetActorLocation().X < DeleteBeforeX)
        {
            if (IsValid(Obstacle))
            {
                Obstacle->Destroy();
            }
            SpawnedObstacles.RemoveAtSwap(Index);
        }
    }
}

void ARoadManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (AActor* Obstacle : SpawnedObstacles)
    {
        if (IsValid(Obstacle))
        {
            Obstacle->Destroy();
        }
    }
    SpawnedObstacles.Reset();
    Super::EndPlay(EndPlayReason);
}
