// Fill out your copyright notice in the Description page of Project Settings.


#include "Food/Chicken.h"

AChicken::AChicken()
{
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/Meshes/Food/Chicken/Raw_Chicken.Raw_Chicken'"));
	//StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
}
