// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <tuple>
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
	UPROPERTY(EditAnywhere)
		int MaxAI;
	UPROPERTY(EditAnywhere)
		int ContainerSize;
	int GoalIndex;
	int SpawnArea;
	std::tuple<int, FVector, int, std::string> GetNewGoal(int groupSize);
	void RegisterGroup(std::vector<std::string> groupMembers);
	std::vector<ATestBasicAI*> bots;
	std::map<int, std::tuple<int,FVector, int, std::string> > instructionMap;
	FVector GoalPosition;
	std::vector<ATestBasicAI*> ReturnBots();
	void RandomCancelSomeGoals();
	void RandomChangeSomeGoals();

	// to pass to ai
	UPROPERTY(EditAnywhere)
		int maxGroupSize;
	UPROPERTY(EditAnywhere)
		int minGroupSize;
	UPROPERTY(EditAnywhere)
		float speedUpper;
	UPROPERTY(EditAnywhere)
		float speedLower;
	UPROPERTY(EditAnywhere)
		float neighbourDistance;
	UPROPERTY(EditAnywhere)
		float avoidanceDistance;
	UPROPERTY(EditAnywhere)
		float outOfBoundsRange;
	UPROPERTY(EditAnywhere)
		float rotationSpeed;

	std::string mergePolicey;
	
	
	std::tuple<int, FVector, int, std::string> AssignGroupTojoin(std::vector<int> choices);

	std::tuple<int, FVector, int, std::string> CheckGoal(int goalID);


	



};
