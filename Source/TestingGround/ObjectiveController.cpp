// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectiveController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Objective.h"
#include "engine.h"
#include <string> 
#include <vector>
using namespace std;

// Sets default values
AObjectiveController::AObjectiveController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NumObjectives = 0;
	MaxObjectives = 5;
	SpawnArea = 10000;


	vector<AObjective*> temp(MaxObjectives);
	objectives = temp;

}

// Called when the game starts or when spawned
void AObjectiveController::BeginPlay()
{
	Super::BeginPlay();


	CreateObjectives();
	
}

// Called every frame
void AObjectiveController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectiveController::CreateObjectives() {

	int count = 0;
	for (int i = 0; i < MaxObjectives; i++) {
		if (NumObjectives < MaxObjectives) {

			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Adding Objective");
			float z = FMath::FRandRange(-SpawnArea, SpawnArea);
			float y = FMath::FRandRange(-SpawnArea, SpawnArea);
			float x = FMath::FRandRange(-SpawnArea, SpawnArea);
			FVector Location(x, y, z);
			FRotator Rotation(0.0f, 0.0f, 0.0f);

			FActorSpawnParameters SpawnInfo;


			//Location = myCharacter->GetActorLocation(); // Get's location of actor 
			AObjective* obj = GetWorld()->SpawnActor<AObjective>(Location, Rotation, SpawnInfo);

			//botArray[AICounter] = newBot;
			objectives[NumObjectives] = obj;
			count += 1;
			NumObjectives += 1;
		}
	}

}

