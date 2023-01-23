// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KitchenAppliance.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

//Forward Declarations
class APlaceableActor;
class  AHelloARManager;
class UKitchenManager;
class UPlayerUI;
/**
 * 
 */

UCLASS()
class UE5_AR_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	
	AHelloARManager* Manager;
	
public:
	ACustomGameMode();
	virtual ~ACustomGameMode() = default;


	virtual void StartPlay() override;

	UPROPERTY(Category="Placeables",EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UUserWidget> InGameUIClass;
	
	/**
	 * @brief An example of an event as generally seen within the Event Graph of Blueprints
	 *  You can have different implementations of this event which will be called when you call this as a delegate.
	 *	See the CPP For more information on the implementation
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "GameModeBase", DisplayName = "Start Play")
		void StartPlayEvent();

	UFUNCTION(BlueprintCallable, Category = "Score")
		int32 GetScore();

	UFUNCTION(BlueprintCallable, Category = "Score")
		int32 GetMoney();

	UFUNCTION(BlueprintCallable, Category="Score")
		void SetScore(const int32 NewScore);

	UFUNCTION(BlueprintCallable, Category="Score")
		void SetMoney(const int32 NewMoney);

	
	virtual void Tick(float DeltaSeconds) override;
	
	

	AHelloARManager* GetManager();

	UKitchenManager* GetKitchenManager();
	

	/**
	 * @brief This function is virtual - you can inherit this class to override this function
	 * Each level can have their own unique spawned actors but with similar base qualities using inheritance
	 */
	virtual void SpawnInitialActors();

	protected:
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UPlayerUI* PlayerUI;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UKitchenManager* KitchenManager;
	
	void LoadUI();

};
