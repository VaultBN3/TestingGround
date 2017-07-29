// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;



	
	
};
