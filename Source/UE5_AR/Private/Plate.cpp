// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"

#include "CustomGameMode.h"
#include "Food.h"
#include "KitchenManager.h"

// Sets default values
APlate::APlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate Mesh Component"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(PlateMesh);
}

// Called when the game starts or when spawned
void APlate::BeginPlay()
{
	Super::BeginPlay();
	PlateMesh->OnComponentHit.AddUniqueDynamic(this,&APlate::OnHit);

	OrderUI = Cast<UCustomerOrderUI>(WidgetComponent->GetWidget());

	if(OrderUI)
	{
		GetWorldTimerManager().SetTimer(Ticker, this, &APlate::SetUI, 0.2f, true, 0.1f);
	}
}


// Called every frame
void APlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlate::SetUI()
{
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);
	auto Orders = GM->GetKitchenManager()->GetOrders();

	//If there is a order in this slot.
	if(Orders.Num() - 1 >= OrderNumber)
	{
		FCustomerOrder Order =  Orders[OrderNumber];
		if(!Order.bCompleted)
		{
			bValidOrder = true;
			OrderUI->SetText(Order.FoodOrdered->FoodName,Order.PreferredCookingMethod,Order.bChopped,Order.FoodOrdered->FoodIcon);
		}
		else
		{
			bValidOrder = false;
			OrderUI->ClearOrder();
		}
	}
	else
	{
		bValidOrder = false;
		OrderUI->ClearOrder();
	}
}



void APlate::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor)
	{
		if(OtherActor->IsA(AFood::StaticClass()))
		{
			AddFood(Cast<AFood>(OtherActor));
		}
	}
}

void APlate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		if(OtherActor->IsA(AFood::StaticClass()))
		{
			RemoveFood(Cast<AFood>(OtherActor));
		}
	}
}

void APlate::AddFood(AFood* FoodToAdd)
{

		FoodOnPlate = FoodToAdd;
		
		auto Temp = GetWorld()->GetAuthGameMode();
		auto GM = Cast<ACustomGameMode>(Temp);

		if(bValidOrder)
		{
			GM->GetKitchenManager()->ScoreOrder(FoodOnPlate,OrderNumber);
		}
	
}

void APlate::RemoveFood(AFood* FoodToRemove)
{

		FoodOnPlate = nullptr;
	
}
