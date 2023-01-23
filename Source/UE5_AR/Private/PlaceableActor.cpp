// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableActor.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"
#include "ProximityMaterialComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlaceableActor::APlaceableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	

	SetActorScale3D(FVector(0.1f,0.1f,0.1f));
}

// Called when the game starts or when spawned
void APlaceableActor::BeginPlay()
{
	Super::BeginPlay();
	PreviousPlaceableScale = 0.1f;
	
}




// Called every frame
void APlaceableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Making sure the actor remains on the ARPin that has been found.
	if(PinComponent)
	{
		auto TrackingState = PinComponent->GetTrackingState();
		
		switch (TrackingState)
		{
		case EARTrackingState::Tracking:
			SceneComponent->SetVisibility(true);
			SetActorLocation(PinComponent->GetLocalToWorldTransform().GetLocation() + OffsetFromARPin);
			//SetActorRotation(PinComponent->GetLocalToWorldTransform().GetRotation());
		

			// Scale down default cube mesh - Change this for your applications.
			break;

		case EARTrackingState::NotTracking:
			PinComponent = nullptr;

			break;

		}
		SetActorLocation(PinComponent->GetLocalToWorldTransform().GetLocation() + OffsetFromARPin);
	}

	

}

void APlaceableActor::SetOffsetFromARPin(FTransform OffsetToSet)
{
	OffsetFromARPin = OffsetToSet.GetLocation() - PinComponent->GetLocalToWorldTransform().GetLocation();
}

void APlaceableActor::SetPlaceableRotation(FVector2d Direction, FVector2d Location)
{
	//Find out how far from the previous registered touch location the touch now is
	DistanceTravelled += FVector2d::Distance(Location,PreviousLocation);
	//Update the previouslocation to the most recent
	PreviousLocation = Location;
	
	
		//Add the rotation as and when it occurs.
		float YawValue = (Direction.X * -1.0);
		//The rotation is added to the placeable.
		SceneComponent->AddRelativeRotation(FRotator(0.0,YawValue,0.0));
	
}

void APlaceableActor::SetPlaceableScale(float NewScaleMapping)
{
	float ClampMappingValue =(FMath::Sign(NewScaleMapping - PreviousScaleMapping) * 0.01) + PreviousPlaceableScale;
	ClampMappingValue = FMath::Clamp(ClampMappingValue,0.1f,0.4f);
	
	PreviousPlaceableScale = ClampMappingValue;
	ScaleVector = FVector(PreviousPlaceableScale,PreviousPlaceableScale,PreviousPlaceableScale);
	SceneComponent->SetWorldScale3D(ScaleVector);
	PreviousScaleMapping = NewScaleMapping;
}

int32 APlaceableActor::GetPrice()
{
	return PriceToBuy;
}



