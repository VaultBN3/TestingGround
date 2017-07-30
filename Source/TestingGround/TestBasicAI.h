// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
#include <utility>
#include "TestBasicAI.generated.h"

UCLASS()
class TESTINGGROUND_API ATestBasicAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestBasicAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ApplyRules(float DeltaTime);
	void GetController();
	float speed;
	float rotationSpeed;
	FVector averageHeading;
	FVector averagePosition;
	float neighbourDistance;
	float speedUpper;
	float speedLower;
	bool turning;
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	bool hasGoal;
	std::pair<FVector, std::string> currentGoal;
	void SetGoal(std::pair<FVector, std::string> newGoal);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	




	
	
};
