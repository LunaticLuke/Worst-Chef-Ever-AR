// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomARPawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ARBlueprintLibrary.h"
#include "ARPin.h"
#include "ChoppingBoard.h"
#include "Camera/CameraComponent.h"
#include "CustomGameMode.h"
#include "Food.h"
#include "HelloARManager.h"
#include "KitchenManager.h"
#include "PlaceableActor.h"
#include "Kismet/GameplayStatics.h"
#include "KitchenAppliances/KitchenStove.h"

// Sets default values
ACustomARPawn::ACustomARPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SetRootComponent(CameraComponent);


	//Setup the arsessionconfig that features the images to track.
	 static ConstructorHelpers::FObjectFinder<UARSessionConfig>
	 ConfigAsset(TEXT("ARSessionConfig'/Game/Blueprints/HelloARSessionConfig.HelloARSessionConfig'"));
	ARConfigAsset = ConfigAsset.Object;
	
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();
	
	UARBlueprintLibrary::StartARSession(ARConfigAsset);
	

	bPlacing = true;
	ApplianceToSpawn = "ServingPoint";
	
	FVector Tilt,RotationRate,Gravity,Acceleration;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->GetInputMotionState(Tilt,RotationRate,Gravity,Acceleration);
	PreviousPhoneMovement = Gravity.Z;
}



// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);

	//Reset the gesture found flag in order to identify what gesture is happening now.
	bFoundGesture = false; 

	//Check for a one finger gesture
	RecogniseOneFingerTouch();
	//Check for a two finger gesture
	RecogniseTwoFingerGesture();
	//If nothing was found, set the enum accordingly.
	if(!bFoundGesture)
	{
		CurrentGesture = Finger_Gesture::None;
	}

	FString test = FString::SanitizeFloat(timeHeldTouch);

	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, *test);

	switch (CurrentGesture)
	{
	case Finger_Gesture::Rotate:
		SelectedAppliance->SetPlaceableRotation(GetFingersDirection(),GetTouchLocation());
	break;
	case Finger_Gesture::Scale:
		SelectedAppliance->SetPlaceableScale(TwoFingerMapping);
	break;
	case Finger_Gesture::Translate: 
		//If one finger is pressed and appliance is selected
		if(SelectedAppliance)
		{
		LineTraceWorldObjects(FVector2d(GetTouchLocation()));	//Use a world trace to check that the player is still touching the appliance in order to drag it.
		}
		//If one finger is pressed but the player is still in the process of holding to select an actor.
		else if(ApplianceToSelect)
		{
			LineTraceWorldObjects(GetTouchLocation()); //Line trace to check the player is still holding on that actor
		}
	break;
	//Do nothing if no gesture.
	default: break;}

	
	//If placing an object or an object is selected
	if(bPlacing || bSelectedAppliance)
	{
		//The planes should be visible.
		GM->GetManager()->ConfigurePlanes(true);
	}
	//Otherwise, hide them
	else
	{
		GM->GetManager()->ConfigurePlanes(false);
	}
	//If a piece of food is currently held, pass through the touch location to it.
	if(SelectedFood)
	{
		SelectedFood->SetScreenPosition(GetTouchLocation());
	}
	
	//PreviousFingerPosition =  GetTouchLocation();
	PhoneMovement();
}	



	


// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind various player inputs to functions
	// There are a few types - BindTouch, BindAxis, and BindEvent.  
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
	PlayerInputComponent->BindAction("OneFingerAction",IE_Pressed,this,&ACustomARPawn::OnScreenTouch);
	PlayerInputComponent->BindAction("OneFingerAction",IE_Released,this,&ACustomARPawn::ReleasedOneFingerTouch);

	PlayerInputComponent->BindAction("TwoFingerAction",IE_Pressed,this,&ACustomARPawn::OnTwoFingerAction);
	PlayerInputComponent->BindAction("TwoFingerAction",IE_Released,this,&ACustomARPawn::OnReleaseTwoFingerAction);

	PlayerInputComponent->BindAxis("TwoFingerMapping",this,&ACustomARPawn::TwoFingerAxis);
}


void ACustomARPawn::OnScreenTouch()
{
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);

	//If this function is triggered, a finger touch has occured.
	bOneFingerPressed = true;
	
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("One Finger Touch"));

	//If the player is trying to spawn in an object
	if(bPlacing)
	{
		//Do an AR plane trace to work out the location at which the object should be spanwed.
		ARPlaneTrace(true,FVector2d(GetTouchLocation()));
	}
	else //If not placing,cast for world objects rather than AR planes.
	{
		//Providing only one finger has been touched, trace for world in order to check if player is still hovering above it.
		if(!bTwoFingerPressed)
		LineTraceWorldObjects(FVector2d(GetTouchLocation()));
	}
}

FVector2d ACustomARPawn::GetTouchLocation()
{
	float XLocation,YLocation;
	bool pressed = false;
		
	UGameplayStatics::GetPlayerController(this,0)->GetInputTouchState(ETouchIndex::Touch1,XLocation,YLocation,pressed);
	return FVector2d(XLocation,YLocation);
}

void ACustomARPawn::ReleasedOneFingerTouch()
{
	//If an appliance was touched but not held long enough to select, assume it was a touch.
	if(ApplianceToSelect)
	{

		if(ApplianceToSelect->IsA(AKitchenAppliance::StaticClass())) //TODO:: Replace With Interface
		{
			//In this case, switch the appliance either on or off.
			Cast<AKitchenAppliance>(ApplianceToSelect)->ToggleSwitch();
		}else if(ApplianceToSelect->IsA(AChoppingBoard::StaticClass()))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("ChoppingBoard"));
			Cast<AChoppingBoard>(ApplianceToSelect)->Chop();
		}
		//Disregard this pointer now.
		ApplianceToSelect = nullptr;
		//Reset the touch timer.
		timeHeldTouch = 0;
	}
	//If a food was held at the time of a finger release, drop the food. 
	if(SelectedFood)
	{
		//The food is no longer selected
		SelectedFood->bSelected = false;
		//Trigger a line trace to the floor from the food to check if physics can be enabled or not.
		SelectedFood->LineTraceToFloor();
		//No food is selected now.
		SelectedFood = nullptr;
	}
	//One finger is no longer pressed,
	bOneFingerPressed = false;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("One Finger Touch Ended"));
}






void ACustomARPawn::LineTraceWorldObjects(FVector2d ScreenPos)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;

	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Emerald, TEXT("Line Trace World Objects"));
	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	UGameplayStatics::DeprojectScreenToWorld(playerController, ScreenPos, WorldPos, WorldDir);
	//Make the Raycast extend for 5 meters. 
	FVector EndPoint = WorldPos + (WorldDir * 500);
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;

	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);

	//If the line trace hit a world collider. 
	if(GetWorld()->LineTraceSingleByChannel(Hit,WorldPos,EndPoint,ECC_Visibility,CollisionParams))
	{
		//If the touch hit some food and a kitchen appliance isnt selected.
		if(Hit.GetActor()->IsA(AFood::StaticClass())&& !bSelectedAppliance)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Hit Food"));
			SelectedFood = Cast<AFood>(Hit.GetActor());

			//If the selected food has been bought, it is good to select so it can be parented to the camera.
			//That hit food is now selected.
			if(SelectedFood->bBought)
			{
				SelectedFood->ParentToCamera();
				SelectedFood->bSelected = true;
			}
			else // Hit food not bought
			{
				//If the player has enough money to buy that food, Buy it
				if(GM->GetMoney() >= SelectedFood->FoodStats->Price)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Buying Food"));
					//Reduce the player's money
					GM->SetMoney(GM->GetMoney() - SelectedFood->FoodStats->Price);
					//The food is bought
					SelectedFood->bBought = true;
					//Hide the price UI
					SelectedFood->DisplayUI(false);
				}
			}
		}else if(Hit.GetActor()->IsA(APlaceableActor::StaticClass()))
		{
			if(bSelectedAppliance) // If already selected... drag
			{
				if(bOneFingerPressed && SingleFingerValidationTime > 0.1f)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Dragging"));
					ARPlaneTrace(false,ScreenPos);
				}
			}
			else //Otherwise, the appliance is now selected.
			{
				//Player has held on placeable for long enough to select it.
				if(timeHeldTouch >= TimeHeldSelectThreshold)
				{
					SelectedAppliance = ApplianceToSelect;
					bSelectedAppliance = true;
					bOneFingerPressed = false;
					ApplianceToSelect = nullptr;
					GM->GetManager()->ConfigurePlanes(true);
					timeHeldTouch = 0;
				}
				else //If holding on appliance but still not long enough
				{
					if(!ApplianceToSelect)
					{
						ApplianceToSelect = Cast<APlaceableActor>(Hit.GetActor());
					}
					timeHeldTouch += GetWorld()->GetDeltaSeconds(); //Increase the time held
				}
			}
			
		}
		else
		{
			//Should be put in a function instead
			timeHeldTouch = 0;
			ApplianceToSelect = nullptr;
			GM->GetManager()->ConfigurePlanes(false);
			bSelectedAppliance = false;
			SelectedAppliance = nullptr;
		}
		
	}
	else
	{
		timeHeldTouch = 0;
		ApplianceToSelect = nullptr;
		GM->GetManager()->ConfigurePlanes(false);
		bSelectedAppliance = false;
		SelectedAppliance = nullptr;
	}

	
}



void ACustomARPawn::ARPlaneTrace(bool bSpawn, FVector2d ScreenPos)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Emerald, TEXT("AR Trace"));

	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), WorldPos, WorldDir);
	// Notice that this LineTrace is in the ARBluePrintLibrary - this means that it's exclusive only for objects tracked by ARKit/ARCore
	auto TraceResult = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(ScreenPos), false, false, false, true);
	//Checks if the location is valid
	if (TraceResult.IsValidIndex(0))
	{
		// Get the first found object in the line trace - ignoring the rest of the array elements
		auto TrackedTF = TraceResult[0].GetLocalToWorldTransform();
		
		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)
		{
			//Spawn the actor pin and get the transform
			//Spawn a new Actor at the location if not done yet
			if (bSpawn)
			{
				
				UARPin* ActorPin = UARBlueprintLibrary::PinComponent(nullptr, TraceResult[0].GetLocalToWorldTransform(), TraceResult[0].GetTrackedGeometry());

				// Check if ARPins are available on your current device. ARPins are currently not supported locally by ARKit, so on iOS, this will always be "FALSE" 
			
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ARPin is valid"));
				//If the pin is valid 
				auto PinTF = ActorPin->GetLocalToWorldTransform();
				const FActorSpawnParameters SpawnInfo;
				const FRotator MyRot(0, 0, 0);
				const FVector MyLoc(0, 0, 0);
				ACustomGameMode* GM = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Reached Hit Plane"));
				auto SpawnedActor = GM->GetKitchenManager()->SpawnAppliance(ApplianceToSpawn,PinTF);
				//SpawnedActor->SetActorLocation(PinTF.GetLocation());
				//SpawnedActor->SetActorRotation(PinTF.GetRotation());
				SpawnedActor->PinComponent = ActorPin;
				GM->SetMoney(GM->GetMoney() - SpawnedActor->GetPrice());
				bPlacing = false;
				ApplianceToSpawn = "";
			}
			else
			{
				if(bSelectedAppliance)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Moving Appliance"));
						
					SelectedAppliance->SetOffsetFromARPin(TraceResult[0].GetLocalToWorldTransform());
				}
			}
		}
	}
}

void ACustomARPawn::OnTwoFingerAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Two Finger Action"));
	TwoFingerValidationTime = 0.0f;
	bTwoFingerPressed = true;
	PreviousFingerPosition = GetTouchLocation();
	StartingFingerPosition = PreviousFingerPosition;
	PreviousFingersDistance = GetFingersDistance();
	
}

void ACustomARPawn::OnReleaseTwoFingerAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(" Released Two Finger Action"));
	bTwoFingerPressed = false;
}

/**
 * @brief Finds the distance between two fingers that are pressing the screen.
 * @return The distance between the two touches as a float
 */
float ACustomARPawn::GetFingersDistance()
{
	auto PController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	
	//Set up vectors to store touch locations for each finger.
	FVector2d Touch1Location,Touch2Location;
	bool Touch1Pressed,Touch2Pressed;
	//Find out where each finger is touching the screen and output it to their respective vectors.
	PController->GetInputTouchState(ETouchIndex::Touch1,Touch1Location.X,Touch1Location.Y,Touch1Pressed);
	PController->GetInputTouchState(ETouchIndex::Touch2,Touch2Location.X,Touch2Location.Y,Touch2Pressed);

	return FVector2d::Distance(Touch1Location,Touch2Location);
}

FVector2d ACustomARPawn::GetFingersDirection()
{
	//Find the direction 
	FVector2d Dir = GetTouchLocation() - PreviousFingerPosition;
	//Get the unit vector of the direction
	Dir.Normalize();
	return Dir;
}

void ACustomARPawn::TwoFingerAxis(float Axis)
{
	TwoFingerMapping = Axis;
}

void ACustomARPawn::PhoneMovement()
{
	
	FVector Tilt,RotationRate,Gravity,Acceleration;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->GetInputMotionState(Tilt,RotationRate,Gravity,Acceleration);
	float CurrentPhoneMovement = Gravity.Z;

	CurrentPhoneMovement *= -1;


	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Current Phone Movement: %f"), CurrentPhoneMovement));
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Previous Phone Movement: %f"), PreviousPhoneMovement));
	
	//Calculate if the phone has moved upwards a certain amount.
	if(CurrentPhoneMovement - PreviousPhoneMovement >= 4 && CurrentPhoneMovement > PreviousPhoneMovement && bCanFlip)
	{
		bCanFlip = false;
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, TEXT("Flip"));
		TArray<AActor*> Stoves;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AKitchenStove::StaticClass(),Stoves);

		int ClosestIndex = 0;
		
		
		if(Stoves.Num() > 0)
		{
			float CurrentDistance = FVector::Distance(GetActorLocation(),Stoves[0]->GetActorLocation());
			for(int i = 1; i < Stoves.Num();i++)
			{
				float Dist = FVector::Distance(GetActorLocation(),Stoves[i]->GetActorLocation());
				if(Dist < CurrentDistance)
				{
					CurrentDistance = Dist;
					ClosestIndex = i;
				}
			}
			Cast<AKitchenStove>(Stoves[ClosestIndex])->FlipFood();
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("No Kitchen Stoves Here")));
		}
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Diff: %f"), CurrentPhoneMovement -  PreviousPhoneMovement));


	PhoneMovementTimer += GetWorld()->GetDeltaSeconds();

	//Provides a delay to allow difference in y movement 
	if(PhoneMovementTimer >= 1.0f)
	{
		PreviousPhoneMovement = CurrentPhoneMovement;
		bCanFlip = true;
		PhoneMovementTimer = 0;
		
	}
		
	
}


void ACustomARPawn::RecogniseTwoFingerGesture()
{

	if(bTwoFingerPressed && SelectedAppliance)
	{
		TwoFingerValidationTime += GetWorld()->GetDeltaSeconds();
		if(TwoFingerValidationTime > 0.2f)
		{
			ApplianceToSelect = nullptr;
			//If the distance between where the two fingers are now is within 0.001 margin to zero..
			if( FMath::IsNearlyEqual(FMath::Abs(PreviousFingersDistance - GetFingersDistance()),0,0.001f))
			{
				
				//Do nothing
			}
			//Otherwise, if the change in finger position is not zero
			else
			{
				if(FMath::Abs(GetFingersDirection().Y) < 0.5)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, TEXT("Rotation Gesture Recognised"));
					timeHeldTouch = 0;
					CurrentGesture = Finger_Gesture::Rotate;
					bFoundGesture = true;
				}
				else
				{
					timeHeldTouch = 0;
					//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, TEXT("Scale Gesture Recognised"));
					CurrentGesture = Finger_Gesture::Scale;
					bFoundGesture = true;
				}
			}
		}
	}
	else
	{
		TwoFingerValidationTime = 0.0f;
	}
}

void ACustomARPawn::RecogniseOneFingerTouch()
{
	if(bOneFingerPressed && !bTwoFingerPressed)
	{
		SingleFingerValidationTime += GetWorld()->GetDeltaSeconds();
		if(SingleFingerValidationTime > 0.2f)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Emerald, TEXT("Line Trace World Objects"));
			CurrentGesture = Finger_Gesture::Translate;
			bFoundGesture = true;
		}
	}
	else
	{
		SingleFingerValidationTime = 0.0;
	}
}



