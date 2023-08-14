#include "OurScene.h"

#include "CesiumGlobeAnchorComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "UECesiumImitator.h"

AOurScene::AOurScene() : CurrentPitch(0), PitchSign(false)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AOurScene::UpdateUavCamera(FVector UavCoordinates, FRotator UavRotation)
{
    FVector UEUavCoordinates;
    FRotator UEUavRotation;
    FMatrix UELocalPlaneRotationMatrix;

    ConvertWorldToUE(UavCoordinates, UavRotation, &UEUavCoordinates, &UEUavRotation,
                     &UELocalPlaneRotationMatrix);

    //CameraView->SetActorLocation(UEUavCoordinates);
    CameraView->SetActorRotation(UEUavRotation);
}

void AOurScene::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> FoundCesiumGeoreferences;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACesiumGeoreference::StaticClass(),
                                          FoundCesiumGeoreferences);

    if (FoundCesiumGeoreferences.Num() == 0)
    {
        UE_LOG(LogUECI, Log, TEXT("Can't find any CesiumGeoreference"));
    }
    else if (FoundCesiumGeoreferences.Num() > 1)
    {
        UE_LOG(LogUECI, Log, TEXT("Found several CesiumGeoreference. Using first one"));
    }

    if (FoundCesiumGeoreferences.Num() > 0)
        CesiumGeoreference = (ACesiumGeoreference*) FoundCesiumGeoreferences[0];

    APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
    OurPlayerController->Possess(CameraView);
}

void AOurScene::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void AOurScene::ConvertWorldToUE(FVector AircraftCoordinates, FRotator AircraftRotation,
                                 FVector* UEAircraftCoordinates, FRotator* UEAircraftRotation,
                                 FMatrix* UELocalPlaneRotationMatrix)
{
    double AircraftLongitude = AircraftCoordinates.X;
    double AircraftLatitude = AircraftCoordinates.Y;
    float AircraftHeight = AircraftCoordinates.Z;

    // Compute the position in UE coordinates
    glm::dvec3 RecalcAircraftCoordinates =
        CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(
            glm::dvec3(AircraftLongitude, AircraftLatitude, AircraftHeight));
    *UEAircraftCoordinates = FVector(RecalcAircraftCoordinates.x, RecalcAircraftCoordinates.y,
                                     RecalcAircraftCoordinates.z);

    *UELocalPlaneRotationMatrix = CesiumGeoreference->ComputeEastSouthUpToUnreal(
        *UEAircraftCoordinates);
    FQuat4d UEAircraftRotationQuat = (*UELocalPlaneRotationMatrix).ToQuat() *
                                     AircraftRotation.Quaternion();
    *UEAircraftRotation = UEAircraftRotationQuat.Rotator();

    double tmpRoll = UEAircraftRotation->Roll;
    double tmpPitch = UEAircraftRotation->Pitch;

    UEAircraftRotation->Roll = tmpPitch;
    UEAircraftRotation->Pitch = tmpRoll;
}

// Called every frame
void AOurScene::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PitchSign)
        CurrentPitch -= 0.1;
    else
        CurrentPitch += 0.1;

    if (CurrentPitch <= -50)
    {
        PitchSign = !PitchSign;
    }

    if (CurrentPitch >= 5)
    {
        PitchSign = !PitchSign;
    }

    UpdateUavCamera(FVector(38.149512, 55.572995, 300), FRotator(0, -90, CurrentPitch));
}
