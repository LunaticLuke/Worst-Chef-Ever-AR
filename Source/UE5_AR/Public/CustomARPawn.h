// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARSessionConfig.h"
#include "CustomActor.h"
#include "Food.h"
#include "KitchenAppliance.h"
#include "Food/FoodDataAsset.h"
#include "GameFramework/Pawn.h"
#include "CustomARPawn.generated.h"

class UCameraComponent;
class APlaceableActor;

UENUM()
enum Finger_Gesture
{
	Rotate UMETA(DisplayName = "Rotate"),
	Scale UMETA(DisplayName = "Scale"),
	Translate UMETA(DisplayName = "Translate"),
	None UMETA(DisplayName = "None"),
	
};


UCLASS()
class UE5_AR_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnScreenTouch();

	FVector2d GetTouchLocation();

	void ReleasedOneFingerTouch();
	/**
	 * @brief This function fires a line trace that hits world objects such as food or kitchen appliances. 
	 * @param ScreenPos The Position at which the screen was touched and the line trace will occur from.
	 */
	void LineTraceWorldObjects(FVector2d ScreenPos);
	/**
	 * @brief 
	 * @param bSpawn This function fires a line trace that hits AR planes
	 * @param ScreenPos The Position at which the screen was touched and the line trace will occur from.
	 */
	void ARPlaneTrace(bool bSpawn, FVector2d ScreenPos);

	void OnTwoFingerAction();

	void OnReleaseTwoFingerAction();

	float GetFingersDistance();

	FVector2d GetFingersDirection();

	void TwoFingerAxis(float Axis);
	
	void PhoneMovement();

	float TwoFingerMapping;

	float PreviousPhoneMovement;

	Finger_Gesture CurrentGesture = Finger_Gesture::None;
	
	/**
	 * @brief This functions determines whether a player is scaling or rotating a placeable object. Called only when two fingers are pressed on the screen.
	 * @return Returns true if rotating and false if scaling.
	 */
	void RecogniseTwoFingerGesture();

	/**
	 * @brief This function checks whether it truly is a one finger touch. This is needed as one finger touch will otherwise trigger before we ever get to check if a second finger has been touched.
	 * @return Whether One finger has been touched.
	 */
	void RecogniseOneFingerTouch();

	float SingleFingerValidationTime = 0;

	float TwoFingerValidationTime = 0;

	FVector2d PreviousFingerPosition;

	FVector2d StartingFingerPosition;

	float PreviousFingersDistance = 0;

	bool bFoundGesture = false;

	float PhoneMovementTimer = 0;

	bool bCanFlip = true;

	AFood* SelectedFood;

	APlaceableActor* SelectedAppliance;

	bool bSelectedAppliance = false;

	//Storing whether the player has pressed the screen with one finger.
	bool bOneFingerPressed = false;
	//Storing whether the player has pressed the screen with two fingers.
	bool bTwoFingerPressed = false;
	
	//Stores how long the press has been held
	float timeHeldTouch = 0;
	//Stores the appliance that has been pressed (Needs to be held on to be fully selected)
	APlaceableActor* ApplianceToSelect;
	//Stores how long the player has to hold a touch on an appliance for it to be selected.
	float TimeHeldSelectThreshold = 0.5f;

	FTimerHandle Ticker;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;


	UARSessionConfig* ARConfigAsset;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UFoodDataAsset*> FoodItems;

	
	FString ApplianceToSpawn;
	bool bPlacing = false;
};
