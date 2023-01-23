// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Food/FoodDataAsset.h"
#include "CustomerOrderUI.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API UCustomerOrderUI : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	UTextBlock* FoodOrderedText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CookingMethodText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ChoppedText;
	FString FoodOrderedString;
	FString CookingMethodString;
	FString ChoppedString;
	UPROPERTY(meta=(BindWidget))
	UImage* FoodIcon;
	void DisplayUI();
public:
	void SetText(FString NameOfFood, ECookingTypes CookingMethod, bool bChopped,UTexture2D* FoodImage);
	void ClearOrder();
};
