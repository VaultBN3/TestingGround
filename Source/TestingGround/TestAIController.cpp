// Fill out your copyright notice in the Description page of Project Settings.

#include "TestAIController.h"
#include "engine.h"
#include "TestBasicAI.h"
#include "Objective.h"
#include "Kismet/KismetSystemLibrary.h"
#include <vector>

using namespace std;



// Sets default values
ATestAIController::ATestAIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AICounter = 0;
	MaxAI = 100;
	vector<ATestBasicAI*> botVector(MaxAI);
	bots = botVector;
	ContainerSize = 1000;


	FVector Location(0.0f, 0.0f,2000.0f);
	GoalPosition = Location;
}

// Called when the game starts or when spawned
void ATestAIController::BeginPlay()
{	

	Super::BeginPlay();
	AllSpawn();
	
}

// Called every frame
void ATestAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//MoveAI();
	if (FMath::RandRange(0, 10000) < 0) {
		float z = FMath::FRandRange(-ContainerSize, ContainerSize);
		float y = FMath::FRandRange(-ContainerSize, ContainerSize);
		float x = FMath::FRandRange(-ContainerSize, ContainerSize);
		FVector NewGoalPosition(x, y, z);
		GoalPosition = NewGoalPosition;

	}

	if (FMath::RandRange(0, 10000) < 100) {

	
		for (TActorIterator<AObjective> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{

			GoalPosition = ActorItr->GetActorLocation();


		}


	}
	

}

void ATestAIController::SpawnAI()
{		
	//ATestBasicAI* botArray[50];
	
	if (AICounter < MaxAI) {
		float z = FMath::FRandRange(-ContainerSize, ContainerSize);
		float y = FMath::FRandRange(-ContainerSize, ContainerSize);
		float x = FMath::FRandRange(-ContainerSize, ContainerSize);
		FVector Location(x, y, z);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;

		ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		//Location = myCharacter->GetActorLocation(); // Get's location of actor 
		ATestBasicAI* newBot = GetWorld()->SpawnActor<ATestBasicAI>(Location, Rotation, SpawnInfo);

		//botArray[AICounter] = newBot;
		bots[AICounter] = newBot;

		AICounter += 1;
	}

}


vector<ATestBasicAI*> ATestAIController::ReturnBots() {

	return bots;

}

void ATestAIController::TimedSpawn() {

		
		FTimerHandle SpawnHandle;
		GetWorldTimerManager().SetTimer(
			SpawnHandle, this, &ATestAIController::SpawnAI, 1.0f, true);

		FTimerHandle MoveHandle;

		GetWorldTimerManager().SetTimer(MoveHandle, this, &ATestAIController::MoveAI, 0.01f, true);
		

}

void ATestAIController::AllSpawn() {

	for (int i = 0; i < MaxAI; i++) {

		SpawnAI();

	}

	//FTimerHandle MoveHandle;

	//GetWorldTimerManager().SetTimer(MoveHandle, this, &ATestAIController::MoveAI, 0.01f, true);




}


void ATestAIController::MoveAI() {
	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector playerCurrent = myCharacter->GetActorLocation();


	// The problem could be that we should be moving per tick, which might lead to smooth movement.
	// can also use move to kismet library 
	//UKismetSystemLibrary::MoveComponentTo(USceneComponent* Component, FVector TargetRelativeLocation, FRotator TargetRelativeRotation, bool bEaseOut, bool bEaseIn, float OverTime, TEnumAsByte<EMoveComponentAction::Type> MoveAction, FLatentActionInfo LatentInfo);

	for (int i = 0; i < AICounter; i += 1) {
		ATestBasicAI* bot = bots[i];
		FVector botCurrent = bot->GetActorLocation();
		FVector move = FMath::VInterpTo(botCurrent, playerCurrent, 0.1f, 0.1f);
		FVector move2 = FMath::Lerp(botCurrent, playerCurrent, 0.1f);
		bot->SetActorLocation(move);
		
		
		




	}





}


