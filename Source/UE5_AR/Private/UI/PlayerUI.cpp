// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUI.h"

#include "CustomARPawn.h"
#include "CustomGameMode.h"
#include "KitchenManager.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Starting Player UI"));
	StoveButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::SelectStove);
	SmallOvenButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::SelectSmallOven);
	ShopButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::OpenShop);
	CloseButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::CloseShop);
	MicrowaveButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::SelectMicrowave);
	ChoppingBoardButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::SelectChoppingBoard);

	PauseButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::PauseGame);
	ResumeButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::ResumeGame);
	RestartButton->OnClicked.AddUniqueDynamic(this,&UPlayerUI::RestartGame);
	CloseShop();
	PauseBackground->SetVisibility(ESlateVisibility::Hidden);
}


void UPlayerUI::SelectStove()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Spawning Stove"));
	
	SpawnAppliance("Stove");
}

void UPlayerUI::SelectSmallOven()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Spawning Small Oven"));
	SpawnAppliance("Oven");
}

void UPlayerUI::SelectMicrowave()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Spawning Microwave"));
	SpawnAppliance("Microwave");
}

void UPlayerUI::SelectChoppingBoard()
{
	SpawnAppliance("ChoppingBoard");
}

void UPlayerUI::OpenShop()
{
	ApplianceScrollBox->SetVisibility(ESlateVisibility::Visible);
	ShopButton->SetVisibility(ESlateVisibility::Collapsed);
	CloseButton->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerUI::CloseShop()
{
	ApplianceScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	ShopButton->SetVisibility(ESlateVisibility::Visible);
	CloseButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerUI::ResumeGame()
{
	PauseBackground->SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::SetGamePaused(GetWorld(),false);
}

void UPlayerUI::PauseGame()
{
	PauseBackground->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::SetGamePaused(GetWorld(),true);
}

void UPlayerUI::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}


void UPlayerUI::SpawnAppliance(FString Appliance)
{
	ACustomGameMode* GM = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM->GetKitchenManager()->HasEnoughMoney(Appliance))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Reached Spawn Appliance Function of UI"));
		ACustomARPawn* Player = Cast<ACustomARPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
		Player->bPlacing = true; //Change to function within PlayerPawn
		Player->ApplianceToSpawn = Appliance;
		CloseShop();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Not enough Money"));
	}
}

void UPlayerUI::SetPlayerMoney(int32 MoneyToSet)
{
	if(MoneyToSet > PlayerMoney)
	{
		MoneyBalanceText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	}
	else
	{
		MoneyBalanceText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
	
	UpdatingTimer = 0;
	bUpdatingText = true;
	PlayerMoney = MoneyToSet;
	
	
}

void UPlayerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(MoneyBalanceText && PlayerMoney)
	{
		MoneyBalanceText->SetText(FText::FromString(FString::FromInt(PlayerMoney))); 
	}

	if(bUpdatingText)
	{
		if(UpdatingTimer < 1)
		{
			UpdatingTimer += GetWorld()->GetDeltaSeconds();
		}
		else
		{
			bUpdatingText = false;
			MoneyBalanceText->SetColorAndOpacity(FSlateColor(FLinearColor(FLinearColor::White)));
		}
	}
}









