// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProximityMaterialComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_AR_API UProximityMaterialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProximityMaterialComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UMaterialInstanceDynamic* DynamicMaterial;

	APawn* PlayerPawn;
	
	UStaticMeshComponent* SMC;

	bool IsCloseToPlayer();

	bool CanAffordItem();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

	UPROPERTY(Category = "Proximity Materials",EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* ProximityMaterialInterface;
};
