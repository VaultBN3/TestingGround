// Fill out your copyright notice in the Description page of Project Settings.

#include "TestAIController.h"
#include "engine.h"
#include "TestBasicAI.h"
#include "Objective.h"
#include "Kismet/KismetSystemLibrary.h"
#include <string>
#include <vector>
#include <utility> 

using namespace std;



// Sets default values
ATestAIController::ATestAIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AICounter = 0;
	MaxAI = 700;
	//440
	vector<ATestBasicAI*> botVector(MaxAI);
	bots = botVector;
	//ContainerSize = 10000;
	ContainerSize = 5000;


	FVector Location(0.0f, 0.0f,2000.0f);
	GoalPosition = Location;
}

// Called when the game starts or when spawned
void ATestAIController::BeginPlay()
{	

	Super::BeginPlay();
	AllSpawn();
	
}


/// This needs to return a (FVector, completeionCondition)
// Needs to keep track of objective, AI count;
// This will allow the controller more tactical decisions regarding the number of AI, group sizes etc. 
// New params, Group_policy-> make new, fill to max, mixture
//				min group size, 
//				max group size

// This will take a group size;
pair<FVector, string> ATestAIController::GetNewGoal() {

	FVector newGoalPos(0, 0, 0);
		if (FMath::RandRange(0, 1) < 1) {
		//	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Random Loc");
			float z = FMath::FRandRange(-ContainerSize, ContainerSize);
			float y = FMath::FRandRange(-ContainerSize, ContainerSize);
			float x = FMath::FRandRange(-ContainerSize, ContainerSize);
			FVector NewGoalPosition(x, y, z);
			newGoalPos = NewGoalPosition;
		}
		else {
			int size = 0;
			for (TActorIterator<AObjective> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{

				size += 1;

			}
			int objectiveSelected = FMath::RandRange(0, size);
			FString IntAsString = FString::FromInt(objectiveSelected);
			//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Target Objective " + IntAsString);
			int count = 0;
			for (TActorIterator<AObjective> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				if (count == objectiveSelected) {
					newGoalPos = ActorItr->GetActorLocation();
				}
				count += 1;


			}

		}

		pair<FVector, string> objective = make_pair(newGoalPos, "WAIT");
		return objective;
	



}
 

// Called every frame
void ATestAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Test");
	//MoveAI();
	if (FMath::RandRange(0, 10000) < 50) {
		if (FMath::RandRange(0, 1) < 1) {
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Random Loc");
			float z = FMath::FRandRange(-ContainerSize, ContainerSize);
			float y = FMath::FRandRange(-ContainerSize, ContainerSize);
			float x = FMath::FRandRange(-ContainerSize, ContainerSize);
			FVector NewGoalPosition(x, y, z);
			GoalPosition = NewGoalPosition;
		}
		else {
			int size = 0;
			for (TActorIterator<AObjective> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				
				size += 1;
				
			}
			int objectiveSelected = FMath::RandRange(0, size);
			FString IntAsString = FString::FromInt(objectiveSelected);
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Target Objective " + IntAsString);
			int count = 0;
			for (TActorIterator<AObjective> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				if (count == objectiveSelected) {
					GoalPosition = ActorItr->GetActorLocation();
				}
				count += 1;


			}

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


