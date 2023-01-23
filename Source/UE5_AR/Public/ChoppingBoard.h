// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableActor.h"
#include "Components/BoxComponent.h"
#include "ChoppingBoard.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API AChoppingBoard : public APlaceableActor
{
	GENERATED_BODY()

public:
	AChoppingBoard();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


	void Chop();
	
protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	class AFood* FoodToInteractWith;
	UPROPERTY(Category= "Meshes",EditAnywhere)
	UBoxComponent* BoxCollisionComponent;
	
	UPROPERTY(Category="Audio",EditAnywhere)
	class USoundBase* ChoppingSound;

	UPROPERTY(Category= "Audio",EditAnywhere)
	class UAudioComponent* ChoppingBoardAudioComponent;
	
};
