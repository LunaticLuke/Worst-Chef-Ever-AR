// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenAppliances/Microwave.h"

#include "Food.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

AMicrowave::AMicrowave()
{


	MicrowaveLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Microwave Light"));
	MicrowaveLightComponent->SetupAttachment(StaticMeshComponent);

	MicrowaveDoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("Microwave Door Pivot"));
	MicrowaveDoorPivot->SetupAttachment(StaticMeshComponent);
	
	MicrowaveDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Microwave Door"));
	MicrowaveDoorMesh->SetupAttachment(MicrowaveDoorPivot);

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate Mesh"));
	PlateMesh->SetupAttachment(StaticMeshComponent);

	TargetFoodDestination = CreateDefaultSubobject<USceneComponent>(TEXT("Target Food Point"));
	TargetFoodDestination->SetupAttachment(StaticMeshComponent);

	PriceToBuy = 20;
}

void AMicrowave::BeginPlay()
{
	Super::BeginPlay();

	MicrowaveLightComponent->RecreateRenderState_Concurrent();
	if(MicrowaveHumSound)
	{
		ApplianceAudioComponent->SetSound(MicrowaveHumSound);
		ApplianceAudioComponent->Stop();
	}
	PlateStartPos = PlateMesh->GetRelativeLocation();
	PlateMesh->OnComponentHit.AddDynamic(this,&AMicrowave::OnHit);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this,&AMicrowave::OnOverlapEnd);
}

void AMicrowave::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(LerpProgress < 1)
	{
	LerpProgress += DeltaSeconds;
	}

	if(FoodToInteractWith)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Emerald, TEXT("FoodInMicrowave"));
	}
		
	if(bSwitchedOn)
	{
		float CurrentDoorRot = FMath::Lerp(MicrowaveDoorPivot->GetRelativeRotation().Yaw,0.0f,LerpProgress);
		MicrowaveDoorPivot->SetRelativeRotation(FRotator(MicrowaveDoorPivot->GetRelativeRotation().Pitch,CurrentDoorRot,MicrowaveDoorPivot->GetRelativeRotation().Roll));
		FVector CurrentPlateLoc = FMath::Lerp(PlateMesh->GetRelativeLocation(),TargetFoodDestination->GetRelativeLocation(),LerpProgress * 2);
		PlateMesh->SetRelativeLocation(CurrentPlateLoc);
	}
	else
	{
		float CurrentDoorRot = FMath::Lerp(MicrowaveDoorPivot->GetRelativeRotation().Yaw,90.0f,LerpProgress);
		MicrowaveDoorPivot->SetRelativeRotation(FRotator(MicrowaveDoorPivot->GetRelativeRotation().Pitch,CurrentDoorRot,MicrowaveDoorPivot->GetRelativeRotation().Roll));
		FVector CurrentPlateLoc = FMath::Lerp(PlateMesh->GetRelativeLocation(),PlateStartPos,LerpProgress);
		PlateMesh->SetRelativeLocation(CurrentPlateLoc);

		if (FoodToInteractWith && LerpProgress >= 1) //Finished Moving Plate Out of microwave, can now detach food.
		{
			FoodToInteractWith->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
}

void AMicrowave::PerformCookingFunction()
{
	if(!FoodAudioComponent->IsPlaying())
	{
		FoodAudioComponent->Play();
	}
	FoodToInteractWith->Cook(5,ECookingTypes::Microwave,false);
	
	if(ApplianceUI)
	{
		ApplianceUI->UpdateUI(FoodToInteractWith->GetCookingProgress());
	}
	
	if(LerpProgress >= 1)
	{
		PlateMesh->AddRelativeRotation(FRotator(0.0f,5.0f * GetWorld()->GetDeltaSeconds(),0.0f));
	}
}

void AMicrowave::SwitchFunctionality()
{
	LerpProgress = 0;
	if(bSwitchedOn)
	{
		MicrowaveLightComponent->SetIntensity(500.0f);
		ApplianceAudioComponent->Play();
		if(FoodToInteractWith)
		{
			FoodToInteractWith->ConfigurePhysics(false);
			FoodToInteractWith->AttachToComponent(PlateMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}else
	{
		MicrowaveLightComponent->SetIntensity(0.0f);
		ApplianceAudioComponent->Stop();
		FoodAudioComponent->Stop();
		
	}
}
