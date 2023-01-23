// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitchenAppliance.h"
#include "Components/PointLightComponent.h"
#include "SmallOven.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API ASmallOven : public AKitchenAppliance
{
	GENERATED_BODY()
	ASmallOven();

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	
	UPROPERTY(Category="Audio",EditAnywhere)
	class USoundBase* OvenHumSound;
	UPROPERTY(Category="Lighting",EditAnywhere)
	UPointLightComponent* OvenLightComponent;
	UPROPERTY(Category="Meshes",EditAnywhere)
	USceneComponent* OvenDoorPivot;
	UPROPERTY(Category="Meshes",EditAnywhere)
	UStaticMeshComponent* OvenDoorMesh;
	//The plate the player will place the food on to cook it.
	UPROPERTY(Category="Meshes",EditAnywhere)
	UStaticMeshComponent* PlateMesh;
	//The point at which the plate will move to.
	UPROPERTY(Category="Meshes",EditAnywhere)
	USceneComponent* TargetFoodDestination;

	float LerpProgress = 0;

	virtual void PerformCookingFunction() override;

	virtual void SwitchFunctionality() override;

	FVector PlateStartPos;
	
private:
	float MaxLightIntensity = 5000.0f;

};
