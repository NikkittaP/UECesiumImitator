// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CesiumGeoreference.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/Tuple.h"

#include "OurScene.generated.h"

USTRUCT(BlueprintType)
struct FMovingVehicleData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float SecondsToCompletePath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FVector> PointCoordinates;
};

USTRUCT(BlueprintType)
struct FFlightPlanData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Altitude;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float SecondsToCompletePath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FVector> PointCoordinates;
};

UCLASS()
class UECESIUMIMITATOR_API AOurScene : public AActor
{
    GENERATED_BODY()

public:
    AOurScene();

    // FVector in degrees and meters (lon; lat; alt)
    // FRotator in degrees (roll; yaw; pitch)
    void UpdateUavCamera(FVector UavCoordinates, FRotator UavRotation);

protected:
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void LoadData();
    void ConvertWorldToUE(FVector AircraftCoordinates, FRotator AircraftRotation,
                          FVector* UEAircraftCoordinates, FRotator* UEAircraftRotation,
                          FMatrix* UELocalPlaneRotationMatrix);

public:
    virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    APawn* CameraView;

    UFUNCTION(BlueprintCallable)
    TArray<FMovingVehicleData> GetMovingVehiclesPathsPoints();

    UFUNCTION(BlueprintCallable)
    FFlightPlanData GetFlightPlanPoints();

    UFUNCTION(BlueprintCallable)
    FQuat InverseTransformRotationQuat(const FTransform& Transform, const FQuat& Rotation);

private:
    ACesiumGeoreference* CesiumGeoreference;

    TArray<FMovingVehicleData> MovingVehiclesPathsPoints;
    FFlightPlanData FlightPlanData;
};
