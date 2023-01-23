// Fill out your copyright notice in the Description page of Project Settings.


#include "Food/Steak.h"

ASteak::ASteak()
{
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/Meshes/Food/Steak/Raw_meat.Raw_meat'"));
	//StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
}
