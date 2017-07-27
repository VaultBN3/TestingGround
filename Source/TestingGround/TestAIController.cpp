// Fill out your copyright notice in the Description page of Project Settings.

#include "TestAIController.h"
#include "engine.h"
#include "TestBasicAI.h"
#include <vector>
using namespace std;



// Sets default values
ATestAIController::ATestAIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AICounter = 0;
	MaxAI = 10;

}

// Called when the game starts or when spawned
void ATestAIController::BeginPlay()
{	

	Super::BeginPlay();
	ControlledSpawn();
	
}

// Called every frame
void ATestAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	
}

void ATestAIController::SpawnAI()
{		
	//ATestBasicAI* botArray[50];
	vector<ATestBasicAI*> botVector(50);


	float y = FMath::FRandRange(-1500, 1500);
	float x = FMath::FRandRange(-1500, 1500);
	FVector Location(x, y, 400.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//Location = myCharacter->GetActorLocation(); // Get's location of actor 
	ATestBasicAI* newBot = GetWorld()->SpawnActor<ATestBasicAI>(Location, Rotation, SpawnInfo);
	
	//botArray[AICounter] = newBot;
	botVector[AICounter] = newBot;

	AICounter += 1;


}

void ATestAIController::ControlledSpawn() {

		
		FTimerHandle SpawnHandle;
		GetWorldTimerManager().SetTimer(
			SpawnHandle, this, &ATestAIController::SpawnAI, 5.0f, true);

		FTimerHandle MoveHandle;

		GetWorldTimerManager().SetTimer(MoveHandle, this, &ATestAIController::MoveAI, 5.0f, true);
		

}

void ATestAIController::MoveAI() {





}


