// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"


#include "ChoppingBoard.h"
#include "CustomARPawn.h"
#include "Plate.h"
#include "ProximityMaterialComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FoodWidgetComponent.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	
	

	FoodUI = CreateDefaultSubobject<UFoodWidgetComponent>(TEXT("Food UI Widget"));
	FoodUI->SetupAttachment(RootComponent);

	FoodMaterialInterface = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("Material'/Game/Assets/Meshes/Food/Master.Master'")).Object;
	
	
	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//CollisionBox->SetupAttachment(StaticMeshComponent);
}

void AFood::ConfigurePhysics(bool EnablePhysics)
{
	StaticMeshComponent->SetSimulatePhysics(EnablePhysics);
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
	//LineTraceToFloor();
	if(FoodStats)
	{
		InitFood(FoodStats);
	}

	if(FoodMaterialInterface)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(FoodMaterialInterface,this);
		StaticMeshComponent->SetMaterial(0,DynamicMaterial);
	}

	//Setup Cooking Makeup Array
	CookingMakeup.Add(ECookingTypes::Microwave,0);
	CookingMakeup.Add(ECookingTypes::Stove,0);
	CookingMakeup.Add(ECookingTypes::Oven,0);

	//Flip(GetActorLocation(),GetActorLocation() + FVector(0,0,100));
}



// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
	if(bSelected)
	{

		
		ACustomARPawn* Player = Cast<ACustomARPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));


		auto PController = UGameplayStatics::GetPlayerController(GetWorld(),0);

		FVector ScreenWorldPos,ScreenWorldDir;
		
		UGameplayStatics::DeprojectScreenToWorld(PController,ScreenPos,ScreenWorldPos,ScreenWorldDir);


		FTransform PlayerTransform = Player->CameraComponent->GetComponentTransform();

		PlayerTransform.SetLocation(ScreenWorldPos);
		
		
		//FMatrix Pawn_Init = Player->CameraComponent->GetComponentTransform().ToMatrixWithScale();
		FMatrix Pawn_Init = PlayerTransform.ToMatrixWithScale();
		
		FMatrix FoodParentedMatrix = FoodRelativeMatrix * Pawn_Init;

		FTransform(FoodParentedMatrix).SetLocation(FVector(FTransform(FoodParentedMatrix).GetLocation().X,ScreenWorldPos.Y,ScreenWorldPos.Z));
		
		

		
		SetActorTransform(FTransform(FoodParentedMatrix));

		StaticMeshComponent->SetSimulatePhysics(false);
	}

	if(bFlipping)
	{
		
		if(FlipProgress < 1)
		{
			float CurrentZPos = FMath::Lerp(GetActorLocation().Z,TargetFoodZ,FlipProgress);
			SetActorLocation(FVector(GetActorLocation().X,GetActorLocation().Y,CurrentZPos));
			FlipProgress += GetWorld()->GetDeltaSeconds() * 2;

			
			float IntervalOfRotation = 180 * (GetWorld()->GetDeltaSeconds() * 2);
			
			AddActorWorldRotation(FRotator(0.0,0.0,IntervalOfRotation));
			
		}
		else
		{
			bFlipping = false;
			FlipProgress = 0;
		}

		if(!bBonusAwarded)
		{
			if(CookingProgress >= 50 && CookingProgress <= 75)
			{
				bBonusAwarded = true;
				Cook(15,CurrentCookingMethod,true);
			}
		}
	}
	else
	{
		if(FlipProgress < 1)
		{
			float CurrentZPos = FMath::Lerp(GetActorLocation().Z,StartingFoodZ,FlipProgress);
			SetActorLocation(FVector(GetActorLocation().X,GetActorLocation().Y,CurrentZPos));
			FlipProgress += GetWorld()->GetDeltaSeconds() * 2;
		}
		else
		{
			if(!bResetPhysics)
			{
				ConfigurePhysics(true);
				bResetPhysics = true;
			}
		
		}
		
	}
	
}



/**
 * @brief This function makes use of matrix transformations to parent the food item to the camera.
 */
void AFood::ParentToCamera()
{
	
	ACustomARPawn* Player = Cast<ACustomARPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));


	auto PController = UGameplayStatics::GetPlayerController(GetWorld(),0);

	FVector ScreenWorldPos,ScreenWorldDir;
		
	UGameplayStatics::DeprojectScreenToWorld(PController,ScreenPos,ScreenWorldPos,ScreenWorldDir);


	FTransform PlayerTransform = Player->CameraComponent->GetComponentTransform();
	PlayerTransform.SetLocation(ScreenWorldPos);

	FMatrix Pawn_Init = PlayerTransform.ToMatrixWithScale();
	//FMatrix Pawn_Init = Player->CameraComponent->GetComponentTransform().ToMatrixWithScale();
	
	
	InverseMatrix =  PlayerTransform.ToInverseMatrixWithScale();
	


	FTransform TestTransform;
	TestTransform.SetLocation(PlayerTransform.GetLocation() + ScreenWorldDir * 50);
	TestTransform.SetRotation(GetActorRotation().Quaternion());
	TestTransform.SetScale3D(GetActorScale3D());
	
	FMatrix FoodMatrix = TestTransform.ToMatrixWithScale();
	
	FoodRelativeMatrix = FoodMatrix * InverseMatrix;
}



void AFood::Cook(float CookingProgressParam,ECookingTypes CookingMethod, bool bInstantAddition)
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, TEXT("Cooking"));
	if(!bInstantAddition)
	{
		CookingProgress += CookingProgressParam * GetWorld()->GetDeltaSeconds();

		CookingMakeup[CookingMethod] += CookingProgressParam * GetWorld()->GetDeltaSeconds();
	}
	else
	{
		CookingProgress += CookingProgressParam;

		CookingMakeup[CookingMethod] += CookingProgressParam;
	}
	

	CurrentCookingMethod = CookingMethod;
	
	if(CookingProgress >= 100 && CookingProgress < 125)
	{
		//StaticMeshComponent->SetStaticMesh(FoodStats->CookedFoodMesh);
		//Set it brownish to simulate cooked
		DynamicMaterial->SetVectorParameterValue("Colour",FLinearColor(0.156f,0.104,0.0206f));
		CookingState = CookedStatus::Cooked;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Cooked"));
	}else if(CookingProgress >= 125)
	{
		DynamicMaterial->SetVectorParameterValue("Colour",FLinearColor::Black);
		CookingState = CookedStatus::Burnt;
		//StaticMeshComponent->SetStaticMesh(FoodStats->BurntFoodMesh);
	}
}

void AFood::DisplayUI(bool ShowUI)
{
	FoodUI->SetVisibility(ShowUI);
}

UFoodDataAsset* AFood::GetFoodStats()
{
	return FoodStats;
}

void AFood::SetScreenPosition(FVector2d ScreenToSet)
{
	ScreenPos = ScreenToSet;
}

void AFood::Chop()
{
	if(!bChopped)
	{
		bChopped = true;
		StaticMeshComponent->SetStaticMesh(FoodStats->ChoppedFoodMesh);
	}
}

void AFood::Flip(FVector StartingPosition,FVector TargetPosition)
{
	//Find the offset between food and pan
	float Diff = GetActorLocation().Z - StartingPosition.Z;
	StartingFoodZ = StartingPosition.Z + Diff;

	TargetFoodZ = TargetPosition.Z + 100;

	bFlipping = true;
	ConfigurePhysics(false);

	FlipProgress = 0;
	bResetPhysics = false;
}

float AFood::GetCookingProgress()
{
	return CookingProgress;
}

void AFood::LineTraceToFloor()
{
	//PointDownwards
	FVector LineDir = FVector(0.0f,0.0f,-1.0f);
	FVector EndPoint = GetActorLocation() + (LineDir * 1000);
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if(GetWorld()->LineTraceSingleByChannel(Hit,GetActorLocation(),EndPoint,ECC_Visibility,CollisionParams))
	{
		//Only enable physics if hovering above an appliance, plate or chopping board...Otherwise levitate.
		if(Hit.GetActor()->IsA(AKitchenAppliance::StaticClass()) || Hit.GetActor()->IsA(APlate::StaticClass()) || Hit.GetActor()->IsA(AChoppingBoard::StaticClass()))
		{
			ConfigurePhysics(true);
		}
	}
	else
	{
		ConfigurePhysics(false);
	}
}

CookedStatus AFood::GetCookingStatus()
{
	return CookingState;
}



void AFood::InitFood(UFoodDataAsset* FoodDataAsset)
{
	FoodStats = FoodDataAsset;
	StaticMeshComponent->SetStaticMesh(FoodStats->FoodMesh);
	FoodUI->SetFoodStats(FoodStats->FoodName,FoodStats->Price);
}

