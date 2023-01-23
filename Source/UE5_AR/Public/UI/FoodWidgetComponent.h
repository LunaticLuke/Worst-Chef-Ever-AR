// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FoodWidgetComponent.generated.h"


class AFood;

UCLASS()
class UE5_AR_API UFoodWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	UFoodWidgetComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	void SetFoodStats(FString Name, int32 Price);
	
};
