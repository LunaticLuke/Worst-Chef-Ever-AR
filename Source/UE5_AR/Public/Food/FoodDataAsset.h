// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FoodDataAsset.generated.h"


UENUM()
enum class ECookingTypes
{
	Microwave UMETA(DisplayName = "Microwave"),
	Stove UMETA(DisplayName = "Stove"),
	Oven UMETA(DisplayName = "Oven")
};
/**
 * 
 */
UCLASS(BlueprintType)
class UE5_AR_API UFoodDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	UStaticMesh* FoodMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	UStaticMesh* ChoppedFoodMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	UMaterialInterface* FoodMaterial;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	UTexture2D* FoodIcon;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats")
	FString FoodName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats")
	int32 Price;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	USoundBase* CookingSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats")
	ECookingTypes OptimalCookingMethod;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats")
	float MaxReward = 0;
};
