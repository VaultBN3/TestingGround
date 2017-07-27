// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBasicAI.h"
#include "engine.h"
#include "TestAIController.h"
#include <vector>

using namespace std;




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
	//mesh->SetSimulatePhysics(true);
	//mesh->BodyInstance.bLockXRotation = true;
	//mesh->BodyInstance.bLockYRotation = true;

	RootComponent = mesh;


	 //speed = FMath::FRandRange(2.0f, 5.0f);

}

// Called when the game starts or when spawned
void ATestBasicAI::BeginPlay()
{
	Super::BeginPlay();
	speed = FMath::FRandRange(100.0f, 300.0f);
	rotationSpeed = 4.0f;
	neighbourDistance = 600.0f;
		
}

// Called every frame
void ATestBasicAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FMath::RandRange(0, 5) < 1) {
		ApplyRules(DeltaTime);
	}

	//this->SetActorRelativeLocation(FVector(0,0,speed*DeltaTime));
	
	this->SetActorLocation(this->GetActorLocation() + ((this->GetActorForwardVector() * speed) * DeltaTime), false);
	
	

	
	//https://wiki.unrealengine.com/Iterators:_Object_%26_Actor_Iterators,_Optional_Class_Scope_For_Faster_Search



	
}

void ATestBasicAI::GetController() {


}
void ATestBasicAI::ApplyRules(float DeltaTime) {
	// Get Other Bot locations
	vector<ATestBasicAI*> bots(50);
	FVector goalPos;

	for (TActorIterator<ATestAIController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		bots = ActorItr->bots;
		goalPos = ActorItr->GoalPosition;


		FVector vcentre;
		FVector vavoid;

		float gSpeed = 0.1f;
		float dist;

		int groupSize = 0;

		for (TActorIterator<ATestBasicAI> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->GetName() != this->GetName()) 
			{
				dist = (this->GetActorLocation() - ActorItr->GetActorLocation()).Size();
				
				if (dist <= neighbourDistance) 
				{
					vcentre = ActorItr->GetActorLocation();
					groupSize++;

					if (dist < 20.0f) 
					{

						vavoid = vavoid + (this->GetActorLocation() - ActorItr->GetActorLocation());
					}

					gSpeed = gSpeed + ActorItr->speed;

				}

			}

		}


		if (groupSize > 0) {

			vcentre = vcentre / groupSize + (goalPos - this->GetActorLocation());
			speed = gSpeed / groupSize;

			FVector direction = (vcentre + vavoid) - this->GetActorLocation();
			if (direction != FVector(0, 0, 0)) {
			

				this->SetActorRotation(FQuat::Slerp(this->GetActorRotation().Quaternion(), direction.ToOrientationQuat(), rotationSpeed * DeltaTime));




			}



		}





	}

}