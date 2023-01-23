// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitchenAppliance.h"
#include "Components/PointLightComponent.h"
#include "Sound/SoundBase.h"
#include "KitchenStove.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API AKitchenStove : public AKitchenAppliance
{
	GENERATED_BODY()


public:
	AKitchenStove();
	

	void FlipFood();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY(Category = "Meshes", VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TemperatureDialMesh;
	UPROPERTY(Category="Meshes",EditAnywhere)
	UStaticMeshComponent* FryingPanMesh;
	UPROPERTY(Category="Audio",EditAnywhere);
	USoundBase* StoveSound;
	UPROPERTY(Category="Lighting",EditAnywhere)
	UPointLightComponent* StoveLightComponent;
	virtual void SwitchFunctionality() override;

	virtual void PerformCookingFunction() override;

	float StartingPanZ;
	float TargetPanZ;

	bool bFlipping = false;

	float FlipProgress = 1;
};
