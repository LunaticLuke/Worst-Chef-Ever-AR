// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoppingBoard.h"

#include "Food.h"
#include "Components/AudioComponent.h"

AChoppingBoard::AChoppingBoard()
{
	ChoppingBoardAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Chopping Board Audio Component"));
	ChoppingBoardAudioComponent->SetupAttachment(StaticMeshComponent);
	
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("Box Collision Component");
	BoxCollisionComponent->SetupAttachment(StaticMeshComponent);

	PriceToBuy = 15;
}

void AChoppingBoard::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->OnComponentHit.AddDynamic(this,&AChoppingBoard::OnHit);
	BoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this,&AChoppingBoard::OnOverlapEnd);
	ChoppingBoardAudioComponent->Stop();
	ChoppingBoardAudioComponent->SetSound(ChoppingSound);
}

void AChoppingBoard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AChoppingBoard::Chop()
{
	if(FoodToInteractWith)
	{
		if(!FoodToInteractWith->bChopped)
		{
			FoodToInteractWith->Chop();
		}
	}
	ChoppingBoardAudioComponent->Play();
}

void AChoppingBoard::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->IsA(AFood::StaticClass()))
	{
		FoodToInteractWith = Cast<AFood>(OtherActor);
	}
}

void AChoppingBoard::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->IsA(AFood::StaticClass()))
	{
		FoodToInteractWith = nullptr;
	}
}
