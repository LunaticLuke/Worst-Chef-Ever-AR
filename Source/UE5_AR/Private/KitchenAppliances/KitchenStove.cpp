// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenAppliances/KitchenStove.h"
#include "Food.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

AKitchenStove::AKitchenStove()
{
	
	
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/Meshes/CookingAppliances/Stove/appliance_stove_appliance_stove.appliance_stove_appliance_stove'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	TemperatureDialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Temperature Dial"));
	auto DialMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/Meshes/CookingAppliances/Stove/appliance_stove_appliance_stove_knob.appliance_stove_appliance_stove_knob'"));
	TemperatureDialMesh->SetStaticMesh(DialMesh.Object);
	TemperatureDialMesh->SetupAttachment(StaticMeshComponent);

	StoveLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Stove Light"));
	StoveLightComponent->SetupAttachment(StaticMeshComponent);

	FryingPanMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frying Pan"));
	FryingPanMesh->SetupAttachment(StaticMeshComponent);

	auto FryingPanAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/Meshes/KitchenUtensils/Pan/pan_frying_pan_pan_frying_pan.pan_frying_pan_pan_frying_pan'"));
	StaticMeshComponent->SetStaticMesh(FryingPanAsset.Object);



	PriceToBuy = 10;
}
void AKitchenStove::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryActorTick.bCanEverTick = true;
	if(StoveSound)
	{
		ApplianceAudioComponent->SetSound(StoveSound);
		ApplianceAudioComponent->Stop();
	}
	StoveLightComponent->RecreateRenderState_Concurrent();
	FryingPanMesh->OnComponentHit.AddDynamic(this,&AKitchenStove::OnHit);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this,&AKitchenStove::OnOverlapEnd);
	
	StartingPanZ = FryingPanMesh->GetRelativeLocation().Z;
	TargetPanZ = StartingPanZ + 400;
	
}

void AKitchenStove::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!bSwitchedOn && FoodAudioComponent->IsPlaying())
	{
		FoodAudioComponent->Stop();
	}

	if(bFlipping)
	{
		if(FlipProgress < 1)
		{
			float CurrentZPos = FMath::Lerp(FryingPanMesh->GetRelativeLocation().Z,TargetPanZ,FlipProgress);
			FryingPanMesh->SetRelativeLocation(FVector(FryingPanMesh->GetRelativeLocation().X,FryingPanMesh->GetRelativeLocation().Y,CurrentZPos));
			FlipProgress += GetWorld()->GetDeltaSeconds() * 2;
		}
		else
		{
			bFlipping = false;
			FlipProgress = 0;
		}
	}
	else
	{
		if(FlipProgress < 1)
		{
			float CurrentZPos = FMath::Lerp(FryingPanMesh->GetRelativeLocation().Z,StartingPanZ,FlipProgress);
			FryingPanMesh->SetRelativeLocation(FVector(FryingPanMesh->GetRelativeLocation().X,FryingPanMesh->GetRelativeLocation().Y,CurrentZPos));
			FlipProgress += GetWorld()->GetDeltaSeconds() * 2;
		}
		
	}

	
}

void AKitchenStove::FlipFood()
{
	if(!bFlipping && FlipProgress >= 1)
	{
		bFlipping = true;
		FlipProgress = 0;
		if(FoodToInteractWith)
		{
			FoodToInteractWith->Flip(FryingPanMesh->GetComponentLocation(),FryingPanMesh->GetComponentLocation() + FVector(0,0,400));
		}
	}
}

void AKitchenStove::SwitchFunctionality()
{
	if(bSwitchedOn)
	{
		ApplianceAudioComponent->Play();
		StoveLightComponent->SetIntensity(1000.0f);
	}else
	{
		ApplianceAudioComponent->Stop();
		StoveLightComponent->SetIntensity(0.0f);
	}
}


void AKitchenStove::PerformCookingFunction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Stove Switched On"));
	if(!FoodAudioComponent->IsPlaying())
	{
		FoodAudioComponent->Play();
	}
	FoodToInteractWith->Cook(5,ECookingTypes::Stove,false);
	if(ApplianceUI)
	{
		ApplianceUI->UpdateUI(FoodToInteractWith->GetCookingProgress());
	}
}
