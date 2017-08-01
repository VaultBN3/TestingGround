// Fill out your copyright notice in the Description page of Project Settings.

#include "Objective.h"
#include "engine.h"


// Sets default values
AObjective::AObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("/Game/StarterContent/Props/MaterialSphere.MaterialSphere")); 
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("/Game/StarterContent/Props/MaterialSphere.MaterialSphere"));
	static ConstructorHelpers::FObjectFinder <UMaterialInterface>Material_Blue(TEXT("/Game/TopDownBP/Blueprints/EmissiveMat.EmissiveMat"));
	

	UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Material_Blue.Object, NULL);
	float bound = 10.0f;

	FLinearColor Red(FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound));
	FLinearColor Green(FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound));
	FLinearColor InterpedColor = FMath::Lerp(Red, Green, FMath::FRandRange(0, bound));
	DynMat->SetVectorParameterValue(FName("colour"), InterpedColor);


	//Material'/Game/TopDownBP/Blueprints/EmissiveMat.EmissiveMat'

	mesh->SetStaticMesh(StaticMesh.Object);
	//mesh->AttachParent = RootComponent;
	//mesh->SetMaterial(0, Material_Blue.Object);
	mesh->SetMaterial(0, DynMat);
	mesh->SetSimulatePhysics(true);
	mesh->SetEnableGravity(false);
	//mesh->BodyInstance.bLockYRotation = true;
	//mesh->BodyInstance.bLockXRotation = true;
	//mesh->BodyInstance.bLockZRotation = true;
	//mesh->BodyInstance.SetDOFLock(EDOFMode::SixDOF);

	

	//mesh->BodyInstance.bLockXRotation = true;
	//mesh->BodyInstance.bLockYRotation = true;
	mesh->SetWorldScale3D(FVector(8.0f, 8.0f, 8.0f));
	RootComponent = mesh;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;


}

// Called when the game starts or when spawned
void AObjective::BeginPlay()
{
	Super::BeginPlay();
	mesh->GetBodyInstance()->bLockYRotation = true;
	mesh->GetBodyInstance()->bLockXRotation = true;
	mesh->GetBodyInstance()->bLockZRotation = true;
	mesh->GetBodyInstance()->bLockRotation = true;
	mesh->GetBodyInstance()->bLockTranslation = true;
	mesh->GetBodyInstance()->bLockXTranslation = true;
	mesh->GetBodyInstance()->bLockYTranslation = true;
	mesh->GetBodyInstance()->bLockZTranslation = true;

	mesh->GetBodyInstance()->SetDOFLock(EDOFMode::SixDOF);
	//mesh->SetCollisionProfileName(TEXT("BlockAll"));
	mesh->BodyInstance.SetCollisionProfileName("TestObjectiveChannel");

	mesh->bGenerateOverlapEvents = true;
	




}

// Called every frame
void AObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

