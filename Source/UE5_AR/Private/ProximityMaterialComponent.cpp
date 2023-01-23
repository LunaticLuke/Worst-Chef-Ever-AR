// Fill out your copyright notice in the Description page of Project Settings.


#include "ProximityMaterialComponent.h"

#include "CustomGameMode.h"
#include "Food.h"
#include "Food/FoodDataAsset.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UProximityMaterialComponent::UProximityMaterialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ProximityMaterialInterface = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("Material'/Game/Assets/Meshes/Food/Master.Master'")).Object;
	// ...
}


// Called when the game starts
void UProximityMaterialComponent::BeginPlay()
{
	Super::BeginPlay();

	if(ProximityMaterialInterface)
	{
		
		DynamicMaterial = UMaterialInstanceDynamic::Create(ProximityMaterialInterface,this);

		SMC = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		if(SMC)
		{
			//SMC->SetMaterial(0,DynamicMaterial);
		}
		PlayerPawn = UGameplayStatics::GetPlayerPawn(this,0);
	}
}


bool UProximityMaterialComponent::IsCloseToPlayer()
{
	
	//Is player within 1 meter?
	if(FVector::Distance(SMC->GetComponentLocation(),PlayerPawn->GetActorLocation()) <= 100)
	{
		
		return true;
	}
	else
	{
		return false;
		
	}
}

bool UProximityMaterialComponent::CanAffordItem()
{
	auto GameMode = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(GameMode);
	auto Food = Cast<AFood>(GetOwner());

	
	if(Food->FoodStats)
	{
		//If the player has enough money
		if(GM->GetMoney() >= Food->FoodStats->Price)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return  false;
	}
	
}

// Called every frame
void UProximityMaterialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if(CanAffordItem())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Can Afford")));
		DynamicMaterial->SetVectorParameterValue("Colour",FLinearColor::Green);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Can't Afford")));
		DynamicMaterial->SetVectorParameterValue("Colour",FLinearColor::Red);
	}*/
	// ...
}

