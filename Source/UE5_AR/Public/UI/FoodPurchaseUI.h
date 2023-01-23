// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FoodPurchaseUI.generated.h"

/**
 * 
 */

UCLASS()
class UE5_AR_API UFoodPurchaseUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	FString PriceValue;

	FString FoodNameValue;
	
public:
	
	
	virtual  void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void SetFoodVariables(FString Name, int32 Price);
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* PriceText;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* FoodNameText;
	
	
	
};
