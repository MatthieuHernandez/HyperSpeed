#include "MapLayout.h"

#include "Engine/Texture2D.h"
#include "ImageCore.h"

#if WITH_EDITOR
#include "UObject/UnrealType.h"
#endif

bool UMapLayout::ImportLayout(UTexture2D* Texture)
{
    Width = Height = 0;
    ObstaclePixels.Reset();
    if (!IsValid(Texture))
    {
        return false;
    }

#if WITH_EDITOR
    if (Texture->Availability != ETextureAvailability::CPU)
    {
        Texture->Modify();
        Texture->Availability = ETextureAvailability::CPU;
        FPropertyChangedEvent Event(
            FindFProperty<FProperty>(UTexture::StaticClass(), GET_MEMBER_NAME_CHECKED(UTexture, Availability)));
        Texture->PostEditChangeProperty(Event);
        Texture->MarkPackageDirty();
        UE_LOG(LogTemp, Display, TEXT("Save %s with Availability=CPU before packaging."), *Texture->GetPathName());
    }
#endif

    const FSharedImageConstRef SourceImage = Texture->GetCPUCopy();
    if (!SourceImage.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("No CPU image for %s. Save the texture with Availability=CPU and rebuild."),
            *Texture->GetPathName());
        return false;
    }

    const int64 PixelCount = int64(SourceImage->SizeX) * SourceImage->SizeY;
    if (SourceImage->SizeX <= 0 || SourceImage->SizeY <= 0 || PixelCount > MAX_int32 || SourceImage->NumSlices != 1)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid obstacle layout dimensions."));
        return false;
    }

    FImage Image;
    SourceImage->CopyTo(Image, ERawImageFormat::BGRA8, EGammaSpace::sRGB);
    Width = Image.SizeX;
    Height = Image.SizeY;
    const auto Pixels = Image.AsBGRA8();
    for (int32 Index = 0; Index < PixelCount; ++Index)
    {
        const FColor& Pixel = Pixels[Index];
        if (Pixel.R == 0 && Pixel.G == 0 && Pixel.B == 0)
        {
            ObstaclePixels.Emplace(Index % Width, Index / Width);
        }
    }
    return true;
}
