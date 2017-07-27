// Fill out your copyright notice in the Description page of Project Settings.

#include "TestAIController.h"
#include "engine.h"
#include "TestBasicAI.h"
#include "Kismet/KismetSystemLibrary.h"
#include <vector>

using namespace std;



// Sets default values
ATestAIController::ATestAIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AICounter = 0;
	MaxAI = 1;
	vector<ATestBasicAI*> botVector(50);
	bots = botVector;
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
	
	if (AICounter < MaxAI) {
		float y = FMath::FRandRange(-500, 500);
		float x = FMath::FRandRange(-500, 500);
		FVector Location(x, y, 400.0f);
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

void ATestAIController::ControlledSpawn() {

		
		FTimerHandle SpawnHandle;
		GetWorldTimerManager().SetTimer(
			SpawnHandle, this, &ATestAIController::SpawnAI, 1.0f, true);

		FTimerHandle MoveHandle;

		GetWorldTimerManager().SetTimer(MoveHandle, this, &ATestAIController::MoveAI, 0.01f, true);
		

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


