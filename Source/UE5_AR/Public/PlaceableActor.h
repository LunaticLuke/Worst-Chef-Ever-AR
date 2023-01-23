// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/Actor.h"
#include "PlaceableActor.generated.h"

class UARPin;

UCLASS()
class UE5_AR_API APlaceableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector ScaleVector;

	float DistanceTravelled;

	FVector2d PreviousLocation;

	float PreviousScaleMapping;

	float PreviousPlaceableScale;

	//Stores the offset at which this actor should be placed from their AR pin.
	FVector OffsetFromARPin = FVector(0,0,0);

	int32 PriceToBuy;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;

	UARPin* PinComponent;

	void SetOffsetFromARPin(FTransform OffsetToSet);

	void SetPlaceableRotation(FVector2d Direction, FVector2d Location);

	void SetPlaceableScale(float NewScaleMapping);

	int32 GetPrice();

};
