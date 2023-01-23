// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FoodWidgetComponent.h"
#include "Food.h"
#include "UI/FoodPurchaseUI.h"

UFoodWidgetComponent::UFoodWidgetComponent()
{
}

void UFoodWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFoodWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFoodWidgetComponent::SetFoodStats(FString Name, int32 Price)
{
	UFoodPurchaseUI* FoodUI = Cast<UFoodPurchaseUI>(GetWidget());
	if(FoodUI)
	{
		FoodUI->SetFoodVariables(Name,Price);
	}
}


