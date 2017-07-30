// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBasicAI.h"
#include "engine.h"
#include "TestAIController.h"
#include "Objective.h"
#include <vector>

using namespace std;




// Sets default values
ATestBasicAI::ATestBasicAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("/Game/StarterContent/Props/MaterialSphere.MaterialSphere")); 
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer"));
	static ConstructorHelpers::FObjectFinder <UMaterialInterface>Material_Blue(TEXT("/Game/TopDownBP/Blueprints/EmissiveMat.EmissiveMat"));


	UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Material_Blue.Object, NULL);
	float bound = 10.0f;

	FLinearColor Red(FMath::FRandRange(0,bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound));
	FLinearColor Green(FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound));
	FLinearColor InterpedColor = FMath::Lerp(Red, Green, FMath::FRandRange(0, bound));
	DynMat->SetVectorParameterValue(FName("colour"), InterpedColor);


	//Material'/Game/TopDownBP/Blueprints/EmissiveMat.EmissiveMat'

	mesh->SetStaticMesh(StaticMesh.Object);
	//mesh->AttachParent = RootComponent;
	//mesh->SetMaterial(0, Material_Blue.Object);
	mesh->SetMaterial(0, DynMat); 
	
	mesh->SetSimulatePhysics(false);
	//mesh->SetCollisionEnabled(TEXT("BlockAll"));
	//mesh->SetCollisionProfileName(TEXT("OverlapAll"));
	

	//mesh->BodyInstance.bLockXRotation = true;
	//mesh->BodyInstance.bLockYRotation = true;

	mesh->SetEnableGravity(false);
	
	

	//mesh->OnComponentHit.Add(this, &ATestBasicAI::OnHit);

	//mesh->SetCollisionProfileName(TEXT("BlockAll"));

	RootComponent = mesh;


	 //speed = FMath::FRandRange(2.0f, 5.0f);

}

// Called when the game starts or when spawned
void ATestBasicAI::BeginPlay()
{
	Super::BeginPlay();

	speedUpper = 400.0f;
	speedLower = 800.0f;
	
	mesh->bGenerateOverlapEvents = true;

	turning = false;

	
	mesh->BodyInstance.SetCollisionProfileName("TestAIChannel");
	//mesh->OnComponentHit.AddDynamic(this, &ATestBasicAI::OnHit);
	mesh->OnComponentBeginOverlap.AddDynamic(this, &ATestBasicAI::OverlapBegin);
	mesh->OnComponentEndOverlap.AddDynamic(this, &ATestBasicAI::OverlapEnd);

	
	speed = FMath::FRandRange(speedLower, speedUpper);

	rotationSpeed = 4.0f;
	neighbourDistance = 500.0f;
	//neighbourDistance = 10000.0f;

	//mesh->SetCollisionProfileName(TEXT("BlockAll"));
		//1000.0f for bit cluster
}

// Called every frame
void ATestBasicAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	


	//if (this->GetActorLocation().Size() >= 10000) {
	//	turning = true;
//	}
//	else {
//		turning = false;
//	}
	
	if (turning) {

		///FVector direction = FVector(0, 0, 0) - this->GetActorLocation();
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Turning");
		
		FVector direction = this->GetActorLocation()*-1;
		this->SetActorRotation(FQuat::Slerp(this->GetActorRotation().Quaternion(), direction.ToOrientationQuat(),1000.0f));
		speed = FMath::FRandRange(speedLower, speedUpper);

	}
	else {
		//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Not Turning");
		// was 5
		if (FMath::RandRange(0, 5) < 1) {
			ApplyRules(DeltaTime);
		}

	}
	FVector goalPos(0, 0, 0);


	for (TActorIterator<AObjective> ActorItr(GetWorld()); ActorItr; ++ActorItr) {


		// Could this be replaced by some sort of proximity event? 

		float dist = (this->GetActorLocation() - ActorItr->GetActorLocation()).Size();
		if (dist <= 700.0f) {

			//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Avoiding objective");

			FVector direction = this->GetActorLocation() - ActorItr->GetActorLocation();
			this->SetActorRotation(FQuat::Slerp(this->GetActorRotation().Quaternion(), direction.ToOrientationQuat(), rotationSpeed * DeltaTime));
			speed = FMath::FRandRange(speedUpper, speedLower);



		}



	}

	this->SetActorLocation(this->GetActorLocation() + ((this->GetActorForwardVector() * speed) * DeltaTime), false);
	
	//https://wiki.unrealengine.com/Iterators:_Object_%26_Actor_Iterators,_Optional_Class_Scope_For_Faster_Search

}

void ATestBasicAI::GetController() {


}


void ATestBasicAI::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit){
	if ((OtherActor != NULL) && OtherActor->IsA(AObjective::StaticClass()))
	{
		//turning = false;
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Hit");

	
	}	

}


void ATestBasicAI::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	//turning = true;
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Overlapping");


}
void ATestBasicAI::OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	//turning = false;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Overlap ended");


}



void ATestBasicAI::ApplyRules(float DeltaTime) {
	// Get Other Bot locations
	vector<ATestBasicAI*> bots(50);
	FVector goalPos;

	for (TActorIterator<ATestAIController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		bots = ActorItr->bots;
		goalPos = ActorItr->GoalPosition;

		FVector vcentre(0,0,0);
		FVector vavoid(0,0,0);

		// gSpeed = speed does some crazy shit
		//float gSpeed = 0.1f;
		float gSpeed = speed;
		float dist;

		int groupSize = 0;

		for (TActorIterator<ATestBasicAI> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->GetName() != this->GetName()) 
			{
				dist = (this->GetActorLocation() - ActorItr->GetActorLocation()).Size();
			
				if (dist <= neighbourDistance) 
				{
					vcentre += ActorItr->GetActorLocation();
					groupSize++;

					// was 2.0f
					// is this bound by size or speeed ? 
					if (dist <= 800.0f) 
					{
						vavoid = vavoid + (this->GetActorLocation() - ActorItr->GetActorLocation());
					}

					gSpeed = gSpeed + ActorItr->speed;

				}
			}
		}


		if (groupSize > 0) {

			vcentre = vcentre / groupSize + (goalPos - this->GetActorLocation());
			speed = gSpeed / groupSize;

			FVector direction = (vcentre + (vavoid)) - this->GetActorLocation();
			
				if (FMath::RandRange(0, 10000) < 100) {
					speed = speed * 10;
					// needs to be on a timer.
				}

				this->SetActorRotation(FQuat::Slerp(this->GetActorRotation().Quaternion(), direction.ToOrientationQuat(), rotationSpeed * DeltaTime));
				speed = FMath::FRandRange(speedUpper, speedLower);
			
		}
	}

}