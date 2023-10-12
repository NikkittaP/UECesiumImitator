// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "UECesiumImitatorGameInstance.generated.h"

UCLASS()
class UECESIUMIMITATOR_API UUECesiumImitatorGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

    /** Evaluates the value of a Runtime Float Curve using the given time. */
    UFUNCTION(BlueprintPure, Category = "Math|RuntimeFloatCurve")
    static float GetRuntimeFloatCurveValue(const FRuntimeFloatCurve& InCurve, float InTime);

    UFUNCTION(BlueprintPure, Category = "Hud")
    float GetObjectScreenRadius(UCameraComponent* InCamera, AActor* InActor);
};