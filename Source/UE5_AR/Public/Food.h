// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Food/FoodDataAsset.h"
#include "GameFramework/Actor.h"
#include "Food.generated.h"

class UProximityMaterialComponent;

UENUM()
enum CookedStatus { Raw,Undercooked,Cooked,Overcooked, Burnt};


//Food base class that handles interactivity and cooking.
UCLASS()
class UE5_AR_API AFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	void ConfigurePhysics(bool EnablePhysics);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FMatrix InverseMatrix;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UFoodWidgetComponent* FoodUI;

	FMatrix FoodRelativeMatrix;

	CookedStatus CookingState = CookedStatus::Raw;

	float CookingProgress = 0;

	FVector2d ScreenPos;

	UPROPERTY(Category = "Materials",EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* FoodMaterialInterface;


	UMaterialInstanceDynamic* DynamicMaterial;

	float StartingFoodZ;
	float TargetFoodZ;

	bool bFlipping = false;

	float FlipProgress = 1;

	bool bBonusAwarded = false;

	ECookingTypes CurrentCookingMethod;

	bool bResetPhysics = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Sets the food's stats
	 * @param FoodDataAsset Data Asset containing meshes, price info
	 */
	void InitFood(UFoodDataAsset* FoodDataAsset);
	/**
	 * @brief Holds Food At Certain Distance From Camera Effectively parenting it.
	 */
	void ParentToCamera();
	/**
	 * @brief Cooks the food 
	 * @param CookingProgressParam How much per second should be added to cooking progress
	 * @param CookingMethod Which appliance is doing the cooking
	 * @param bInstantAddition Should the progress be added in one frame rather than over time.
	 */
	void Cook(float CookingProgressParam, ECookingTypes CookingMethod, bool bInstantAddition);

	void DisplayUI(bool ShowUI);

	UFoodDataAsset* GetFoodStats();

	void SetScreenPosition(FVector2d ScreenToSet);
	/**
	 * @brief Swaps out non chopped mesh with chopped mesh and sets the food to chopped
	 */
	void Chop();

	/**
	 * @brief Starts the process of flipping the food whilst in the stove.
	 * @param StartingPosition Starting Position Of pan - food will be higher
	 * @param TargetPosition Target position of pan - food will be higher
	 */
	void Flip(FVector StartingPosition, FVector TargetPosition);

	float GetCookingProgress();

	UPROPERTY(EditAnywhere)
	///Dictionary that stores the cooking method and how much of the cooking was done via that method - factors into scoring.
	TMap<ECookingTypes,float> CookingMakeup;
	/**
	 * @brief Fires a raycast downwards from the food mesh to check if a surface is beneath it. If there is, physics are enabled.
	 */
	void LineTraceToFloor();
	/**
	 * @brief Finds out how cooked the food item is.
	 * @return Returns the enum specifying how cooked the food is in a range from raw to burnt.
	 */
	CookedStatus GetCookingStatus();
	UPROPERTY(Category = "Audio", EditAnywhere, BlueprintReadWrite)
	class UFoodDataAsset* FoodStats; //Todo Getters and setters
	
	bool bSelected = false;

	bool bBought = false;

	bool bChopped = false;

};
