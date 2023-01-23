// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "UI/CustomerOrderUI.h"
#include "Plate.generated.h"

UCLASS()
class UE5_AR_API APlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PlateMesh;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AddFood(class AFood* FoodToAdd);

	void RemoveFood(AFood* FoodToRemove);

	AFood* FoodOnPlate;
	UPROPERTY(EditAnywhere)
	UWidgetComponent* WidgetComponent;

	UCustomerOrderUI* OrderUI;

	FTimerHandle Ticker;
	UPROPERTY(EditAnywhere)
	//Which Order does this plate correspond to?
	int OrderNumber = 0;

	bool bValidOrder = false;

	void SetUI();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};

