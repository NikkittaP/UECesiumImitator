// Fill out your copyright notice in the Description page of Project Settings.

#include "UECesiumImitatorGameInstance.h"

#include "Camera/CameraComponent.h"

#if WITH_EDITOR
/* Get Screen Percentage */
static const auto CVarScreenPercentage = IConsoleManager::Get().FindTConsoleVariableDataFloat(
    TEXT("r.SCreenPercentage"));
#endif WITH_EDITOR

void UUECesiumImitatorGameInstance::Init()
{
    Super::Init();
}

void UUECesiumImitatorGameInstance::Shutdown()
{
    Super::Shutdown();
}

float UUECesiumImitatorGameInstance::GetRuntimeFloatCurveValue(const FRuntimeFloatCurve& InCurve,
                                                         float InTime)
{
    // If an external curve is provided, we'll use that to evaluate
    if (InCurve.ExternalCurve != nullptr)
    {
        return InCurve.ExternalCurve->GetFloatValue(InTime);
    }

    // If no external curves are provided, we'll evaluate from the rich curve directly
    return InCurve.GetRichCurveConst()->Eval(InTime);
}

float UUECesiumImitatorGameInstance::GetObjectScreenRadius(UCameraComponent* InCamera, AActor* InActor)
{
    float ScreenRadius;
    int32 Width, Height;
    FVector Viewlocation;
    FRotator ViewRotation; // Not Used, but required for Function call
#if WITH_EDITOR
    float ScreenPerc = CVarScreenPercentage->GetValueOnGameThread() / 100.0f;
#endif WITH_EDITOR

    /* Get the size of the viewport, and the player cameras location. */
    GetFirstLocalPlayerController()->GetViewportSize(Width, Height);
    GetFirstLocalPlayerController()->GetPlayerViewPoint(Viewlocation, ViewRotation);

#if WITH_EDITOR
    /* Factor in Screen Percentage & Quality Settings */
    Width *= ScreenPerc;
    Height *= ScreenPerc;
#endif WITH_EDITOR

    /* Easy Way To Return The Size, Create a vector and scale it. Alternative would be to use FMath::Max3 */
    float SRad = FVector2D(Width, Height).Size();

    /* Get Object Bounds (R) */
    float BoundingRadius = InActor->GetRootComponent()->Bounds.SphereRadius;
    float DistanceToObject = FVector(InActor->GetActorLocation() - Viewlocation).Size();

    /* Get Projected Screen Radius */
    ScreenRadius = FMath::Atan(BoundingRadius / DistanceToObject);
    ScreenRadius *= SRad / FMath::DegreesToRadians(InCamera->FieldOfView);

    return ScreenRadius;
}