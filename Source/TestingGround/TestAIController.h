// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
#include <vector>
#include <utility>
#include <map>
#include "TestBasicAI.h"
#include "TestAIController.generated.h"

UCLASS()
class TESTINGGROUND_API ATestAIController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnAI();
	void TimedSpawn();
	void AllSpawn();
	void MoveAI();
	int AICounter;
	int MaxAI;
	int ContainerSize;
	int GoalIndex;
	std::pair<FVector, int> GetNewGoal();
	void RegisterGroup(std::vector<std::string> groupMembers);
	std::vector<ATestBasicAI*> bots;
	std::map<int, std::string> instructionMap;
	FVector GoalPosition;
	std::vector<ATestBasicAI*> ReturnBots();
	void CancelSomeGoals();

	



};
