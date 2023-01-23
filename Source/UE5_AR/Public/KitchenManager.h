// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableActor.h"
#include "Components/ActorComponent.h"
#include "Food/FoodDataAsset.h"
#include "KitchenManager.generated.h"


USTRUCT()
struct FCustomerOrder
{
	GENERATED_BODY()
	UFoodDataAsset* FoodOrdered;
	ECookingTypes PreferredCookingMethod;
	bool bChopped = false;
	bool bCompleted = false;
};


class AFood; //Forward Declarations
class UFoodDataAsset;

//The KitchenManager handles all spawning and data assigning operations regarding kitchen appliances and food. 
UCLASS()
class UE5_AR_API UKitchenManager : public UActorComponent{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	UKitchenManager();

	/**
	 * @brief This function spawns the appliance with the corresponding tag
	 * @param Appliance The string of the appliance to spawn
	 * @param ApplianceTransform Where it should be spawned
	 * @return The spawned appliance
	 */
	APlaceableActor* SpawnAppliance(FString Appliance, FTransform ApplianceTransform);

	/**
	 * @brief This function spawns the food with the corresponding tag
	 * @param Food The string of the appliance to spawn
	 * @param FoodTransform Where it should be spawned
	 * @return The spawned food
	 */
	AFood* SpawnFood(FString Food, FTransform FoodTransform);

	bool HasEnoughMoney(FString Food);


	void InitManager(int32* MoneyToSet);

	/**
	 * @brief Adds a new order to the list providing there isnt already a max amount reached
	 */
	void CreateOrder();

	TArray<FCustomerOrder> GetOrders();

	/**
	 * @brief Starts Off With 100% Of Max Reward & Reduces percentage if player has missed objectives
	 * @param FoodOnPlate The food to score
	 * @param IndexToCheck The order number this food pertains to
	 */
	void ScoreOrder(AFood* FoodOnPlate, int IndexToCheck);

	/**
 * @brief This function cycles through the images tracked within the scene and spawns the specific food mesh on it.
 * If the mesh has already been spawned, move the location to where the image is being tracked already.
 */
	void CheckForTrackedImages();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TMap<FString,TSubclassOf<APlaceableActor>> SpawnableAppliances; //Dictionary that stores all of the appliance types and assigns a string value pair.
	
	UPROPERTY(EditAnywhere)
	TMap<FString,AFood*> SpawnableFood; //Dictionary that stores all of the food types and assigns a string value pair.
	
	//A map of bools to store which images have been found that frame - accessed by a string key that is the food name
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FString,bool> FoundFoodTracker;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UFoodDataAsset*> FoodDataAssets;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AFood> FoodToSpawn;

	int32* PlayerMoney = 0;
	
	//An array of each customer order.
	TArray<FCustomerOrder> Orders;
	//Maximum number of orders at any one time.
	int MaxOrders = 3;
	//Stores which recipe is currently opened by the player. This order is what will be checked when the player serves a plate.
	int CurrentSelectedRecipe = 0;

	bool CanOrderBeCreated(int& OutIndexThatCanBeReplaced);

	FTimerHandle Ticker;
	
};
