// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "KitchenApplianceUI.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API UKitchenApplianceUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
protected:
	UPROPERTY(meta=(BindWidget))
	USlider* CookingProgressSlider;
	UPROPERTY(meta=(BindWidget))
	UImage* FoodIconImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* FlipText;


	FSlateBrush DefaultImage;
public:
	void SetFoodImage(UTexture2D* ImageToSet);
	
	void UpdateUI(float CookingProg);

	void ResetUI();
	
};
