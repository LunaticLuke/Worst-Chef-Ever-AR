// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FoodPurchaseUI.h"
#include "Food.h"


void UFoodPurchaseUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	
}

void UFoodPurchaseUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(PriceText)
	{
		PriceText->SetText(FText::FromString(PriceValue));
	}

	if(FoodNameText)
	{
		FoodNameText->SetText(FText::FromString(FoodNameValue));
	}
}

void UFoodPurchaseUI::SetFoodVariables(FString Name, int32 Price)
{
	PriceValue = FString::FromInt(Price);
	FoodNameValue = Name;
}




