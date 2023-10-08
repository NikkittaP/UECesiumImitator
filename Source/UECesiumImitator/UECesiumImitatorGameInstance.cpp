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