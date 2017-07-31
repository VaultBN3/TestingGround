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
	MaxAI = 400;
	//440
	vector<ATestBasicAI*> botVector(MaxAI);
	bots = botVector;
	//ContainerSize = 10000;
	SpawnArea = 5000;
	ContainerSize = 5000;
	GoalIndex = 0;


	FVector Location(0.0f, 0.0f,2000.0f);
	GoalPosition = Location;


	maxGroupSize = 50;
	minGroupSize = 20;
	speedUpper = 400.0f;
	speedLower = 800.0f;;
	neighbourDistance = 2000.0f;
	avoidanceDistance = 800.0f;
	outOfBoundsRange = 200000;
	rotationSpeed = 4.0f;

	mergePolicey = "CREATE_NEW";
}

// Called when the game starts or when spawned
void ATestAIController::BeginPlay()
{	

	Super::BeginPlay();


	std::map<int, std::tuple<int,FVector, int, std::string> > instructionMap;
	GoalIndex = 0;
	
	

	
	AllSpawn();


	
}





/// This needs to return a (FVector, completeionCondition)
// Needs to keep track of objective, AI count;
// This will allow the controller more tactical decisions regarding the number of AI, group sizes etc. 
// New params, Group_policy-> make new, fill to max, mixture
//				min group size, 
//				max group size

// This will take a group size;

// Brain can control speed of group in some way, maximum speed of all AI = something, speeed will effect other stats, health, damage etc. 
// Brain will assign random avoidance measur to allow for more elaborate formations
tuple<int, FVector, int, string> ATestAIController::GetNewGoal(int groupSize) {

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

		tuple<int, FVector, int, string> objective = make_tuple(GoalIndex, newGoalPos, groupSize, "EXECUTING");
		
		//tuple<FVector, int, string> objectiveLog = make_tuple(newGoalPos, groupSize, "EXECUTING");
		instructionMap[GoalIndex] = objective;
		GoalIndex += 1;

		FString IntAsString = FString::FromInt(GoalIndex);
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Assigned group goal ID " + IntAsString);



		return objective;
	



}

tuple<int, FVector, int, string>  ATestAIController::AssignGroupTojoin(vector<int> choices) {

	tuple<int, FVector, int, string> instructionLog;
	instructionLog = instructionMap[choices[0]];
	tuple<int, FVector, int, string> choice = make_tuple(get<0>(instructionLog), get<1>(instructionLog), get<2>(instructionLog), get<3>(instructionLog));


	return choice;

}

tuple<int, FVector, int, string> ATestAIController::CheckGoal(int goalID) {

	return instructionMap[goalID];

}

void ATestAIController::RandomCancelSomeGoals() {

	for (map<int, tuple<int,FVector, int, string>> ::iterator iter = instructionMap.begin(); iter != instructionMap.end(); ++iter)
	{	
		if (FMath::RandRange(0, 10000) < 50) {
			int k = iter->first;
			get<3>(instructionMap[k]) = "CANCELLED";


		}


	}




}

void ATestAIController::RandomChangeSomeGoals() {



	for (map<int, tuple<int, FVector, int, string>> ::iterator iter = instructionMap.begin(); iter != instructionMap.end(); ++iter)
	{
		if (FMath::RandRange(0, 10000) < 50) {
			int k = iter->first;
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



			get<1>(instructionMap[k]) = newGoalPos;


		}


	}

}
 

// Called every frame
void ATestAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//RandomCancelSomeGoals();
	//RandomChangeSomeGoals();

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Test");
	//MoveAI();
	if (FMath::RandRange(0, 10000) < 50) {
		if (FMath::RandRange(0, 1) < 1) {
			//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Random Loc");
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
			//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Target Objective " + IntAsString);
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
		float z = FMath::FRandRange(-SpawnArea, SpawnArea);
		float y = FMath::FRandRange(-SpawnArea, SpawnArea);
		float x = FMath::FRandRange(-SpawnArea, SpawnArea);
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


