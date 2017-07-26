// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBasicAI.h"
#include "engine.h"



// Sets default values
ATestBasicAI::ATestBasicAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer"));
	static ConstructorHelpers::FObjectFinder <UMaterialInterface>Material_Blue(TEXT("/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer"));

	mesh->SetStaticMesh(StaticMesh.Object);
	//mesh->AttachParent = RootComponent;
	mesh->SetMaterial(0, Material_Blue.Object);
	mesh->SetSimulatePhysics(true);

	RootComponent = mesh;


}

// Called when the game starts or when spawned
void ATestBasicAI::BeginPlay()
{
	Super::BeginPlay();

	
}



// Called every frame
void ATestBasicAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

