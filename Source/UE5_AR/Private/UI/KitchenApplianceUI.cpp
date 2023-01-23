// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KitchenApplianceUI.h"


void UKitchenApplianceUI::NativeConstruct()
{
	Super::NativeConstruct();
	CookingProgressSlider->SetMaxValue(150);
	CookingProgressSlider->SetSliderBarColor(FLinearColor::Gray);

	DefaultImage = FoodIconImage->Brush;
}

void UKitchenApplianceUI::SetFoodImage(UTexture2D* ImageToSet)
{
	FoodIconImage->SetBrushFromTexture(ImageToSet,false);
	FlipText->SetVisibility(ESlateVisibility::Hidden);
}

void UKitchenApplianceUI::UpdateUI(float CookingProg)
{
	CookingProgressSlider->SetValue(CookingProg);

	if(CookingProg >= 50 && CookingProg <= 75)
	{
		FlipText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FlipText->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if(CookingProg >= 100)
	{
		if(CookingProg <= 125)
		{
			CookingProgressSlider->SetSliderHandleColor(FLinearColor::Green);
		}
		else
		{
			CookingProgressSlider->SetSliderHandleColor(FLinearColor::Red);
		}
	}
	
}

void UKitchenApplianceUI::ResetUI()
{
	CookingProgressSlider->SetValue(0);
	CookingProgressSlider->SetSliderHandleColor(FLinearColor::Gray);
	FoodIconImage->SetBrush(DefaultImage);
}

