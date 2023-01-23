// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MoneyBalanceText;
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* ApplianceScrollBox;
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* PauseBackground;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* StoveButton;
	UPROPERTY(meta=(BindWidget))
	UButton* SmallOvenButton;
	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ShopButton;
	UPROPERTY(meta=(BindWidget))
	UButton* MicrowaveButton;
	UPROPERTY(meta=(BindWidget))
	UButton* PauseButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ResumeButton;
	UPROPERTY(meta=(BindWidget))
	UButton* RestartButton;
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;
	UPROPERTY(meta=(BindWidget))
	UButton* ChoppingBoardButton;

	void SpawnAppliance(FString Appliance);

	void SetPlayerMoney(int32 MoneyToSet);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/**
	 * @brief This function is bound to a UI button and starts the process of spawning a stove
	 */
	UFUNCTION(BlueprintCallable)
    	void SelectStove();
	/**
	 * @brief This function is bound to a UI button and starts the process of spawning an oven
	 */
	UFUNCTION(BlueprintCallable)
    	void SelectSmallOven();
	/**
	 * @brief This function is bound to a UI button and starts the process of spawning a microwave
	 */
	UFUNCTION(BlueprintCallable)
		void SelectMicrowave();

	/**
	 * @brief This function is bound to a UI button and starts the process of spawning a chopping board
	 */
	UFUNCTION(BlueprintCallable)
		void SelectChoppingBoard();
	/**
	 * @brief This function is bound to a UI button and opens the shop UI
	 */
	UFUNCTION(BlueprintCallable)
		void OpenShop();
	/**
	 * @brief This function is bound to a UI button and closes the shop UI
	 */
	UFUNCTION(BlueprintCallable)
		void CloseShop();
	UFUNCTION()
		void ResumeGame();
	UFUNCTION()
		void PauseGame();
	UFUNCTION()
		void RestartGame();



protected:

	virtual void NativeConstruct() override;
	

	int32 PlayerMoney;

	bool bUpdatingText;

	float UpdatingTimer = 0;
	
};
