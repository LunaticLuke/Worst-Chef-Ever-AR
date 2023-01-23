// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomerOrderUI.h"


void UCustomerOrderUI::DisplayUI()
{
	FoodOrderedText->SetText(FText::FromString(FoodOrderedString));
	CookingMethodText->SetText(FText::FromString(CookingMethodString));
	ChoppedText->SetText(FText::FromString(ChoppedString));
}

void UCustomerOrderUI::SetText(FString NameOfFood, ECookingTypes CookingMethod, bool bChopped, UTexture2D* FoodImage)
{
	FoodOrderedString = NameOfFood;

	switch (CookingMethod)
	{
	case ECookingTypes::Microwave: CookingMethodString = "Microwave"; break;
	case ECookingTypes::Stove: CookingMethodString = "Stove"; break;
	case ECookingTypes::Oven: CookingMethodString = "Oven"; break;
	default: ;
	}

	ChoppedString = bChopped ? "Chopped" : "Not Chopped";

	FoodIcon->SetBrushFromTexture(FoodImage);
	FoodIcon->SetVisibility(ESlateVisibility::Visible);
	
	DisplayUI();
}

void UCustomerOrderUI::ClearOrder()
{
	FoodOrderedString = "";

	CookingMethodString = "";

	ChoppedString = "";

	FoodIcon->SetVisibility(ESlateVisibility::Hidden);

	DisplayUI();
}
