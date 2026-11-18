#include "MusicEngine.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Internationalization/Regex.h"

void UMusicEngine::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    liveFile = "live.strudel";
    variablesFile = "variables.strudel";
    SetMasterVolume(1.0f);
}

bool UMusicEngine::load(FString& fileContent, const FString& file)
{
    auto filePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/strudel-autoplay"), file);
    if (!FFileHelper::LoadFileToString(fileContent, *filePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to load Strudel file: %s"), *filePath);
        return false;
    }
    return true;
}

bool UMusicEngine::save(const FString& fileContent, const FString& file)
{
    auto filePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/strudel-autoplay"), file);
    if (!FFileHelper::SaveStringToFile(fileContent, *filePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to load Strudel file: %s"), *filePath);
        return false;
    }
    if (file == liveFile)
    {
        UpdateCurrentBPM(fileContent);
    }
    return true;
}

void UMusicEngine::UpdateCurrentBPM(const FString& Content)
{
    // Strudel cycles are treated as four beats, matching the project's music files.
    CurrentBPM = 0.0f;
    const FRegexPattern TempoPattern(
        TEXT(R"((?m)^\s*set(cp[ms])\(\s*(GLOBAL_TEMPO|[0-9]+(?:\.[0-9]+)?)\s*(?:/\s*([0-9]+(?:\.[0-9]+)?))?\s*\))"));
    FRegexMatcher Tempo(TempoPattern, Content);
    while (Tempo.FindNext())
    {
        float Value = 0.0f;
        if (Tempo.GetCaptureGroup(2) == TEXT("GLOBAL_TEMPO"))
        {
            const FRegexPattern GlobalPattern(TEXT(R"(const\s+GLOBAL_TEMPO\s*=\s*([0-9]+(?:\.[0-9]+)?))"));
            FRegexMatcher Global(GlobalPattern, Content);
            if (!Global.FindNext())
            {
                continue;
            }
            Value = FCString::Atof(*Global.GetCaptureGroup(1));
        }
        else
        {
            Value = FCString::Atof(*Tempo.GetCaptureGroup(2));
        }
        const FString DivisorText = Tempo.GetCaptureGroup(3);
        const float Divisor = DivisorText.IsEmpty() ? 1.0f : FCString::Atof(*DivisorText);
        if (Divisor > 0.0f)
        {
            CurrentBPM = Value / Divisor * 4.0f * (Tempo.GetCaptureGroup(1) == TEXT("cps") ? 60.0f : 1.0f);
        }
    }
}

void UMusicEngine::writeLive(const FString& musicFile)
{
    FString variablesContent;
    FString musicContent;
    if (load(variablesContent, variablesFile) && load(musicContent, musicFile))
    {
        const FString liveContent = variablesContent + TEXT("\n") + musicContent + TEXT("\n");
        save(liveContent, liveFile);
    }
}

void UMusicEngine::SetMasterVolume(float value, const FString& file)
{
    FString fileContent;
    if (load(fileContent, *file))
    {
        const FRegexPattern Pattern(TEXT(R"(const\s+MASTER_VOLUME\s*=\s*[\d.]+;)"));
        FRegexMatcher Matcher(Pattern, fileContent);
        if (Matcher.FindNext())
        {
            FString Replacement = FString::Printf(TEXT("const MASTER_VOLUME = %.2f;"), value);
            fileContent =
                fileContent.Left(Matcher.GetMatchBeginning()) + Replacement + fileContent.Mid(Matcher.GetMatchEnding());
            save(fileContent, *file);
        }
    }
}

void UMusicEngine::SetGlobalTempo(int32 value, const FString& file)
{
    FString fileContent;
    if (load(fileContent, *file))
    {
        const FRegexPattern Pattern(TEXT(R"(const\s+GLOBAL_TEMPO\s*=\s*[\d.]+;)"));
        FRegexMatcher Matcher(Pattern, fileContent);
        if (Matcher.FindNext())
        {
            FString Replacement = FString::Printf(TEXT("const GLOBAL_TEMPO = %i;"), value);
            fileContent =
                fileContent.Left(Matcher.GetMatchBeginning()) + Replacement + fileContent.Mid(Matcher.GetMatchEnding());
            save(fileContent, *file);
        }
    }
}

void UMusicEngine::SetMasterVolume(float value)
{
    SetMasterVolume(value, variablesFile);
    SetMasterVolume(value, liveFile);
}

void UMusicEngine::SetGlobalTempo(int32 value)
{
    SetGlobalTempo(value, variablesFile);
    SetGlobalTempo(value, liveFile);
}

void UMusicEngine::SetTempoFromSpeed(float SpeedMetersPerSecond)
{
    const int32 BPM = FMath::Max(0, FMath::RoundToInt(SpeedMetersPerSecond * 5.0f));
    if (FMath::IsNearlyEqual(CurrentBPM, static_cast<float>(BPM)))
    {
        return;
    }

    FString Content;
    if (!load(Content, liveFile))
    {
        return;
    }

    // Normalize the active track to four beats per cycle, regardless of its original tempo syntax.
    const FRegexPattern TempoPattern(TEXT(R"((?m)^\s*setcp[ms]\([^\r\n)]*\))"));
    FRegexMatcher Tempo(TempoPattern, Content);
    if (!Tempo.FindNext())
    {
        return;
    }
    const FString Command = FString::Printf(TEXT("setcpm(%d / 4)"), BPM);
    Content = Content.Left(Tempo.GetMatchBeginning()) + Command + Content.Mid(Tempo.GetMatchEnding());
    save(Content, liveFile);
}

void UMusicEngine::PlayMenuMusic()
{
    writeLive(TEXT("menu.strudel"));
}

void UMusicEngine::PlayMusic(const FString& MusicFile)
{
    if (!MusicFile.IsEmpty())
    {
        writeLive(MusicFile);
    }
}

void UMusicEngine::PlayDesertMusic()
{
    SetGlobalTempo(60, variablesFile);
    writeLive(TEXT("desert.strudel"));
}
