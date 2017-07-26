// Fill out your copyright notice in the Description page of Project Settings.

#include "TestAIController.h"
#include "engine.h"
#include "TestBasicAI.h"



// Sets default values
ATestAIController::ATestAIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestAIController::BeginPlay()
{	

	Super::BeginPlay();
	SpawnAI();
	
}

// Called every frame
void ATestAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	
}

void ATestAIController::SpawnAI()
{
	

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	
	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Location = myCharacter->GetActorLocation();
	GetWorld()->SpawnActor<ATestBasicAI>(Location, Rotation, SpawnInfo);




}


