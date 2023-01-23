// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenAppliance.h"

#include "Food.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

AKitchenAppliance::AKitchenAppliance()
{
	PrimaryActorTick.bCanEverTick = true;

	ApplianceAudioComponent = CreateDefaultSubobject<UAudioComponent>("ApplianceAudioComponent");
	ApplianceAudioComponent->SetupAttachment(StaticMeshComponent);

	FoodAudioComponent = CreateDefaultSubobject<UAudioComponent>("FoodAudioComponent");
	FoodAudioComponent->SetupAttachment(StaticMeshComponent);
	FoodAudioComponent->bOverrideAttenuation = true;
	FoodAudioComponent->AttenuationOverrides.OmniRadius = 115;
	FoodAudioComponent->AttenuationOverrides.AttenuationShapeExtents = FVector(115.0f,0.0f,0.0f);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(StaticMeshComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget Component");
	WidgetComponent->SetupAttachment(StaticMeshComponent);
}

void AKitchenAppliance::BeginPlay()
{
	Super::BeginPlay();

	WidgetComponent->SetHiddenInGame(true);
	
	SetActorScale3D(FVector(PreviousPlaceableScale,PreviousPlaceableScale,PreviousPlaceableScale));

	//Get the UI class directly from the widget component 
	ApplianceUI = Cast<UKitchenApplianceUI>(WidgetComponent->GetWidget());
	//Check it was properly set in the blueprint editor. if not, output a warning.
	if(!ApplianceUI)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Appliance UI Not Configured"));
	}
}

void AKitchenAppliance::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if(bSwitchedOn && FoodToInteractWith)
	{
		PerformCookingFunction();
	}
	
}

/**
 * @brief Changes The Appliance's On Status To The Opposite
 */
void AKitchenAppliance::ToggleSwitch()
{
	bSwitchedOn = !bSwitchedOn;
	SwitchFunctionality();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Reached Switch Method"));
}



/**
 * @brief Overridable function designed to carry out whatever function the inherited appliance does
 * E.g Blending, Frying,Cooking.
 */
void AKitchenAppliance::PerformCookingFunction()
{
}

/**
 * @brief This function allows each individual appliance to show that they have been switched on/off in a unique way.
 * E.g (Lights Coming On For The Microwave Or Lever Pulled Down On Toaster) Designed To Be Overwritten. Called When Switched On Or Off By The Player.
 */
void AKitchenAppliance::SwitchFunctionality()
{
}

void AKitchenAppliance::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->IsA(AFood::StaticClass()))
	{
		FoodToInteractWith = Cast<AFood>(OtherActor);
		WidgetComponent->SetHiddenInGame(false);
		if(ApplianceUI)
		{
			ApplianceUI->SetFoodImage(FoodToInteractWith->GetFoodStats()->FoodIcon);
		}
		FoodAudioComponent->SetSound(FoodToInteractWith->GetFoodStats()->CookingSound);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Collided"));
	}
}

void AKitchenAppliance::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->IsA(AFood::StaticClass()))
	{
		FoodToInteractWith = nullptr;
		ApplianceUI->ResetUI();
		WidgetComponent->SetHiddenInGame(true);
	}
}




