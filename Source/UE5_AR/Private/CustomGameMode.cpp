// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"
#include "CustomARPawn.h"
#include "CustomGameState.h"
#include "CustomActor.h"
#include "HelloARManager.h"
#include "ARBlueprintLibrary.h"
#include "KitchenManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlaceableActor.h"
#include "Blueprint/UserWidget.h"
#include "UI/PlayerUI.h"

ACustomGameMode::ACustomGameMode()
{
	// Add this line to your code if you wish to use the Tick() function
	PrimaryActorTick.bCanEverTick = true;

	// Set the default pawn and gamestate to be our custom pawn and gamestate programatically
	DefaultPawnClass = ACustomARPawn::StaticClass();
	GameStateClass = ACustomGameState::StaticClass();


	//static ConstructorHelpers::FClassFinder<UUserWidget> InGameUIBPClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_PlayerUI.BP_PlayerUI'"));
	KitchenManager = CreateDefaultSubobject<UKitchenManager>(TEXT("Kitchen Manager"));
	
}


void ACustomGameMode::StartPlay() 
{
	SpawnInitialActors();

	// This is called before BeginPlay
	StartPlayEvent();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Current Score: %d"), GetScore()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Current Money: %d"), GetMoney()));

	
	LoadUI();
	FInputModeGameAndUI InputMode;

	UGameplayStatics::GetPlayerController(GetWorld(),0)->SetInputMode(InputMode);
	SetMoney(GetMoney());
	KitchenManager->InitManager(&GetGameState<ACustomGameState>()->MoneyBalance);
	// This function will transcend to call BeginPlay on all the actors 
	Super::StartPlay();
}

// An implementation of the StartPlayEvent which can be triggered by calling StartPlayEvent() 
void ACustomGameMode::StartPlayEvent_Implementation() 
{
	// Start a timer which will call the SpawnCube Function every 4 seconds
	//GetWorldTimerManager().SetTimer(Ticker, this, &ACustomGameMode::SpawnCube, 4.0f, true, 0.0f);
}

int32 ACustomGameMode::GetScore()
{
	return GetGameState<ACustomGameState>()->Score;
}

int32 ACustomGameMode::GetMoney()
{
	return GetGameState<ACustomGameState>()->MoneyBalance;
}

void ACustomGameMode::SetScore(const int32 NewScore)
{
	GetGameState<ACustomGameState>()->Score = NewScore;

}

void ACustomGameMode::SetMoney(const int32 NewMoney)
{
	GetGameState<ACustomGameState>()->MoneyBalance = NewMoney;
	PlayerUI->SetPlayerMoney(NewMoney);
}



void ACustomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	KitchenManager->CheckForTrackedImages();
}


void ACustomGameMode::SpawnInitialActors()
{
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(0, 0, 0);

	// Spawn an instance of the HelloARManager class
	Manager = GetWorld()->SpawnActor<AHelloARManager>(MyLoc,MyRot,SpawnInfo);
	Manager->ConfigurePlanes(false);
}

void ACustomGameMode::LoadUI()
{
	if(InGameUIClass == nullptr) return;

	PlayerUI = CreateWidget<UPlayerUI>(UGameplayStatics::GetPlayerController(GetWorld(),0),InGameUIClass);
	
	if(PlayerUI == nullptr) return;

	PlayerUI->AddToViewport();
	
}




AHelloARManager* ACustomGameMode::GetManager()
{
	return Manager;
}

UKitchenManager* ACustomGameMode::GetKitchenManager()
{
	return KitchenManager;
}





