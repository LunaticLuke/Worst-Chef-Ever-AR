// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitchenAppliance.h"
#include "Components/PointLightComponent.h"
#include "Microwave.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API AMicrowave : public AKitchenAppliance
{
	GENERATED_BODY()
	AMicrowave();

public:

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY(Category="Audio",EditAnywhere)
	class USoundBase* MicrowaveHumSound;
	UPROPERTY(Category="Meshes",EditAnywhere)
	UStaticMeshComponent* MicrowaveDoorMesh;
	UPROPERTY(Category="Lighting",EditAnywhere)
	UPointLightComponent* MicrowaveLightComponent;
	UPROPERTY(Category="Meshes",EditAnywhere)
	USceneComponent* MicrowaveDoorPivot;
	//The plate the player will place the food on to cook it.
	UPROPERTY(Category="Meshes",EditAnywhere)
	UStaticMeshComponent* PlateMesh;
	//The point at which the plate will move to.
	UPROPERTY(Category="Meshes",EditAnywhere)
	USceneComponent* TargetFoodDestination;

	FVector PlateStartPos;

	float LerpProgress = 0;

	virtual void PerformCookingFunction() override;

	virtual void SwitchFunctionality() override;
};
