// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableActor.h"
#include "Components/WidgetComponent.h"
#include "UI/KitchenApplianceUI.h"
#include "KitchenAppliance.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API AKitchenAppliance : public APlaceableActor
{
	GENERATED_BODY()

public:
	AKitchenAppliance();
	
	void ToggleSwitch();

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	
	virtual void PerformCookingFunction();
	
	bool bSwitchedOn = false;
	
	virtual void SwitchFunctionality();

	UPROPERTY(Category= "Audio",EditAnywhere)
	class UAudioComponent* ApplianceAudioComponent;
	UPROPERTY(Category= "Audio",EditAnywhere)
	UAudioComponent* FoodAudioComponent;
	UPROPERTY(Category="UI",EditAnywhere)
	UWidgetComponent* WidgetComponent;

	UKitchenApplianceUI* ApplianceUI;

	class AFood* FoodToInteractWith;

	UPROPERTY(Category="Collision",EditAnywhere)
	class UBoxComponent* CollisionBox;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
