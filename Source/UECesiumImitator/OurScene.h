// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CesiumGeoreference.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "OurScene.generated.h"

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
	void ConvertWorldToUE(FVector AircraftCoordinates, FRotator AircraftRotation,
		FVector* UEAircraftCoordinates, FRotator* UEAircraftRotation,
		FMatrix* UELocalPlaneRotationMatrix);

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		APawn* CameraView;

private:
	ACesiumGeoreference* CesiumGeoreference;

	float CurrentPitch;
	bool PitchSign;
};
