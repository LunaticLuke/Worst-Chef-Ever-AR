// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenAppliances/SmallOven.h"

#include "Food.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"


ASmallOven::ASmallOven()
{
	PriceToBuy = 25;

	
	
	OvenLightComponent = CreateDefaultSubobject<UPointLightComponent>("Oven Light");
	OvenLightComponent->SetupAttachment(StaticMeshComponent);


	OvenDoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("Oven Door Pivot"));
	OvenDoorPivot->SetupAttachment(StaticMeshComponent);
	
	OvenDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Oven Door Mesh"));
	OvenDoorMesh->SetupAttachment(OvenDoorPivot);

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate Mesh"));
	PlateMesh->SetupAttachment(StaticMeshComponent);

	TargetFoodDestination = CreateDefaultSubobject<USceneComponent>(TEXT("Target Food Point"));
	TargetFoodDestination->SetupAttachment(StaticMeshComponent);
}



void ASmallOven::BeginPlay()
{
	Super::BeginPlay();
	OvenLightComponent->RecreateRenderState_Concurrent();
	PlateMesh->OnComponentHit.AddDynamic(this,&ASmallOven::OnHit);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this,&ASmallOven::OnOverlapEnd);
	PlateStartPos = PlateMesh->GetRelativeLocation();
}

void ASmallOven::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if(LerpProgress < 1)
	{
		LerpProgress += DeltaSeconds;
	}

	
	if(bSwitchedOn)
	{
		float CurrentRot = FMath::Lerp(OvenDoorPivot->GetRelativeRotation().Roll,0.0f,LerpProgress);
		OvenDoorPivot->SetRelativeRotation(FRotator(OvenDoorPivot->GetRelativeRotation().Pitch,OvenDoorPivot->GetRelativeRotation().Yaw,CurrentRot));
		FVector CurrentPlateLoc = FMath::Lerp(PlateMesh->GetRelativeLocation(),TargetFoodDestination->GetRelativeLocation(),LerpProgress * 2);
		PlateMesh->SetRelativeLocation(CurrentPlateLoc);
	}
	else
	{
		float CurrentRot = FMath::Lerp(OvenDoorPivot->GetRelativeRotation().Roll,95.0f,LerpProgress);
		OvenDoorPivot->SetRelativeRotation(FRotator(OvenDoorPivot->GetRelativeRotation().Pitch,OvenDoorPivot->GetRelativeRotation().Yaw,CurrentRot));
		FVector CurrentPlateLoc = FMath::Lerp(PlateMesh->GetRelativeLocation(),PlateStartPos,LerpProgress);
		PlateMesh->SetRelativeLocation(CurrentPlateLoc);
	}
}

void ASmallOven::PerformCookingFunction()
{
	if(!FoodAudioComponent->IsPlaying())
	{
		FoodAudioComponent->Play();
	}
	FoodToInteractWith->Cook(5,ECookingTypes::Oven,false);

	if(ApplianceUI)
	{
		ApplianceUI->UpdateUI(FoodToInteractWith->GetCookingProgress());
	}
}

void ASmallOven::SwitchFunctionality()
{
	LerpProgress = 0;
		if(bSwitchedOn)
		{
			
			OvenLightComponent->SetIntensity(5000.0f);
			if(FoodToInteractWith)
			{
				FoodToInteractWith->ConfigurePhysics(false);
				FoodToInteractWith->AttachToComponent(PlateMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
		}
		else
		{
			FoodAudioComponent->Stop();
			OvenLightComponent->SetIntensity(0.0f);
		}
	
}


