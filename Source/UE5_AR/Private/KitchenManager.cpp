// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenManager.h"

#include "ARBlueprintLibrary.h"
#include "CustomGameMode.h"
#include "Food.h"
#include "KitchenAppliance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
UKitchenManager::UKitchenManager()
{
}

// Called when the game starts or when spawned
void UKitchenManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(Ticker, this, &UKitchenManager::CreateOrder, 7.0f, true, 10.0f); //Create Orders on a 7 second interval (Max of 3 at a time)
	
}

bool UKitchenManager::CanOrderBeCreated(int &OutIndexThatCanBeReplaced)
{
	for(int i = 0; i < Orders.Num();i++)
	{
		if(Orders[i].bCompleted)
		{
			OutIndexThatCanBeReplaced = i;
			return true;
		}
	}
	return  false;
}

APlaceableActor* UKitchenManager::SpawnAppliance(FString Appliance, FTransform ApplianceTransform)
{
	if(SpawnableAppliances.Num() > 0)
	{
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot = ApplianceTransform.Rotator();
		const FVector MyLoc = ApplianceTransform.GetLocation();
		
		TSubclassOf<APlaceableActor> FoundAppliance = *SpawnableAppliances.Find(Appliance);

		APlaceableActor* SpawnedAppliance = GetWorld()->SpawnActor<APlaceableActor>(FoundAppliance,MyLoc,MyRot,SpawnInfo);

		return SpawnedAppliance;
	}
	else
	{
		return nullptr;
	}
}

AFood* UKitchenManager::SpawnFood(FString Food, FTransform FoodTransform)
{
	if(SpawnableFood.Num() > 0)
	{
		//Find the food using the friendly name tag from the tracked image
		AFood* FoundFood = *SpawnableFood.Find(Food);
		//If a mesh is already present for this food
		if(FoundFood)
		{
			//Set it false
			FoundFood->SetHidden(false);
			FoundFood->SetActorLocation(FoodTransform.GetLocation());
			FoundFood->SetActorRotation(FoodTransform.GetRotation());
			return FoundFood;
		}
		//No Mesh Present, spawn one in. 
		else
		{
			const FActorSpawnParameters SpawnInfo;
			const FRotator MyRot = FoodTransform.Rotator();
			const FVector MyLoc = FoodTransform.GetLocation();
			//Spawn a food mesh in
			FoundFood = GetWorld()->SpawnActor<AFood>(FoodToSpawn,MyLoc,MyRot,SpawnInfo);
			//Loop through and find the appropriate data asset to initialize the food. 
			for(int i = 0; i < FoodDataAssets.Num();i++)
			{
				if(FoodDataAssets[i]->FoodName == Food) //If its a match
				{
					FoundFood->InitFood(FoodDataAssets[i]);
					SpawnableFood[Food] = FoundFood;
					return FoundFood;
				}
			}
		}
	}
	
	//Something has went wrong if reached this point.
	return nullptr;
}

bool UKitchenManager::HasEnoughMoney(FString Appliance)
{
	TSubclassOf<APlaceableActor>* FoundAppliance = SpawnableAppliances.Find(Appliance);

	if(FoundAppliance)
	{
		if(*PlayerMoney >= FoundAppliance->GetDefaultObject()->GetPrice())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void UKitchenManager::InitManager(int32* MoneyToSet)
{
	PlayerMoney = MoneyToSet;
}

void UKitchenManager::CreateOrder()
{
	//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Cyan, TEXT("Running Create Order"));
	int ReplaceableIndex;
	
	if(Orders.Num() + 1 <= MaxOrders || CanOrderBeCreated(ReplaceableIndex))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Cyan, TEXT("Running Create Order If"));
		int RandomIndex = FMath::RandRange(0,FoodDataAssets.Num() - 1);

		FCustomerOrder Order;
		Order.FoodOrdered = FoodDataAssets[RandomIndex];

		RandomIndex =  FMath::RandRange(0,100);
		if(RandomIndex <= 50)
		{
			Order.bChopped = true;
		}
		RandomIndex = FMath::RandRange(0,2);

		switch (RandomIndex)
		{
		case 0: Order.PreferredCookingMethod = ECookingTypes::Microwave; break;
		case 1: Order.PreferredCookingMethod = ECookingTypes::Oven; break;
		case 2: Order.PreferredCookingMethod = ECookingTypes::Stove; break;
		}
	
	
		if(Orders.Num() + 1 <= MaxOrders)
		{
			Orders.Push(Order);
		}
		else
		{
			if(CanOrderBeCreated(ReplaceableIndex))
			{
				Orders[ReplaceableIndex] = Order;
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Cyan, Orders[0].FoodOrdered->FoodName);
		//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Cyan, UEnum::GetValueAsString(Orders[0].PreferredCookingMethod));
		//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Cyan, Orders[0].bChopped ? "True" : "False");
	}
}

TArray<FCustomerOrder> UKitchenManager::GetOrders()
{
	return Orders;
}

void UKitchenManager::ScoreOrder(AFood* FoodOnPlate, int IndexToCheck)
{
	//Start off with player getting 100% of the reward for meal.
	float PercentageOfReward  = 100;

	//Empty plate check
	if(Orders[IndexToCheck].FoodOrdered != FoodOnPlate->FoodStats)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Cyan, TEXT("No Points"));
		//If empty or wrong, player gets zero reward.
		PercentageOfReward = 0;
	}
	
		if(FoodOnPlate->GetCookingStatus() == CookedStatus::Raw)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Cyan, TEXT("PENALTY: Raw Food"));
			PercentageOfReward *= 0.1; //Half the reward due to it being raw.
		}
		else if(FoodOnPlate->GetCookingStatus() == CookedStatus::Burnt)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Cyan, TEXT("PENALTY: Burnt Food"));
			PercentageOfReward *= 0.25; //Reward is now 3/4 removed due to it being burnt.
		}
	
	
	float IncorrectCooking = 0;
	float CorrectCooking = 0;
	//Iterate through the TMap
	for (const TPair<ECookingTypes, float >& pair : FoodOnPlate->CookingMakeup)
	{
		if(pair.Key == Orders[IndexToCheck].PreferredCookingMethod)
		{
			CorrectCooking += pair.Value;
		}
		else
		{
			IncorrectCooking += pair.Value;
		}
	}
	
	if(IncorrectCooking > 10) //Penalized for cooking with incorrect cooking method
	{
		PercentageOfReward *= 0.5f;
	}

	
	if(Orders[IndexToCheck].bChopped)
	{
		if(!FoodOnPlate->bChopped)
		{
			//25% penalization for not chopping when requested.
			PercentageOfReward *= 0.75f;
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Correct Cooking: %f"), CorrectCooking));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Incorrect Cooking: %f"), IncorrectCooking));

	
	int AmountToAdd = FMath::RoundToInt((Orders[IndexToCheck].FoodOrdered->MaxReward/100) * PercentageOfReward);
	auto GM = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->SetMoney(GM->GetMoney() + AmountToAdd);
	
	Orders[IndexToCheck].bCompleted = true;
	FoodOnPlate->Destroy();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f Percent  Of %d = %d"),PercentageOfReward,FMath::RoundToInt(Orders[CurrentSelectedRecipe].FoodOrdered->MaxReward), FMath::RoundToInt((Orders[CurrentSelectedRecipe].FoodOrdered->MaxReward/100) * PercentageOfReward)));
}

void UKitchenManager::CheckForTrackedImages()
{
	
	
	//Get all of the images tracked within the scene
    	auto Images = UARBlueprintLibrary::GetAllGeometriesByClass<UARTrackedImage>();
	
    
    	auto Temp = GetWorld()->GetAuthGameMode();
    	auto GM = Cast<ACustomGameMode>(Temp);
	
    	//Cycle through the images to find specific foods.
    	for(int i = 0; i < Images.Num(); i++)
    	{
    		UARCandidateImage*  TrackedImage = Images[i]->GetDetectedImage();
    		
    		//Check that the image was successfully detected without a null pointer.
    		if(TrackedImage)
    		{
    			
    			AFood* FoodMesh = SpawnFood(TrackedImage->GetFriendlyName(),Images[i]->GetLocalToWorldTransform());
    			FoundFoodTracker[TrackedImage->GetFriendlyName()] = true;
    			
    			if(FoodMesh->bBought && FoodMesh->bSelected)
    			{
    				SpawnableFood[TrackedImage->GetFriendlyName()] = nullptr;
    			}
    			
    		}
    	}
	
}




