// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBasicAI.h"
#include "engine.h"
#include "TestAIController.h"
#include "Objective.h"
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <utility> 

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

	FLinearColor Red(FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound), FMath::FRandRange(0, bound));
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

	for (TActorIterator<ATestAIController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		
		speedUpper = ActorItr->speedUpper;

		speedLower = ActorItr->speedLower;
		
		maxGroupSize = ActorItr->maxGroupSize;
		
		minGroupSize = ActorItr->minGroupSize;
		
		rotationSpeed = ActorItr->rotationSpeed;
		
		neighbourDistance = ActorItr->neighbourDistance;

		avoidanceDistance = ActorItr->avoidanceDistance;

		outOfBoundsRange = ActorItr->outOfBoundsRange;

		randomSpacing = ActorItr->randomSpacing;
		
		randomGroupSize = ActorItr->randomGroupSize;

		randomSpacingUpper = ActorItr->randomSpacingUpper;

		randomSpacingLower = ActorItr->randomSpacingLower;


	}

	//speedUpper = 400.0f;
	//speedLower = 800.0f;

	mesh->bGenerateOverlapEvents = true;

	turning = false;


	mesh->BodyInstance.SetCollisionProfileName("TestAIChannel");
	//mesh->OnComponentHit.AddDynamic(this, &ATestBasicAI::OnHit);
	mesh->OnComponentBeginOverlap.AddDynamic(this, &ATestBasicAI::OverlapBegin);
	mesh->OnComponentEndOverlap.AddDynamic(this, &ATestBasicAI::OverlapEnd);


	speed = FMath::FRandRange(speedLower, speedUpper);

	//rotationSpeed = 4.0f;
	//neighbourDistance = 2000.0f;


	hasGoal = false;
	hasGroup = false;
	currentGoal = make_tuple(-1, FVector(0, 0, 0), -1, "");
	//maxGroupSize = 50;
	//minGroupSize = 10;

	//neighbourDistance = 10000.0f;

	//mesh->SetCollisionProfileName(TEXT("BlockAll"));
		//1000.0f for bit cluster


	//FString IntAsString = FString::SanitizeFloat(speedUpper);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Speed Upper " + IntAsString);
	//IntAsString = FString::SanitizeFloat(speedLower);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Speed Lower " + IntAsString);
	//IntAsString = FString::FromInt(maxGroupSize);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Max group " + IntAsString);
	//IntAsString = FString::FromInt(minGroupSize);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Max group " + IntAsString);
	//IntAsString = FString::SanitizeFloat(neighbourDistance);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Neighbour " + IntAsString);



}

// Called every frame
void ATestBasicAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (this->GetActorLocation().Size() >= outOfBoundsRange) {
		FVector direction = FVector(0, 0, 0) - this->GetActorLocation();
		this->SetActorRotation(FQuat::Slerp(this->GetActorRotation().Quaternion(), direction.ToOrientationQuat(), 1000.0f));
		speed = FMath::FRandRange(speedLower, speedUpper);
	}
	//	else {
	//		turning = false;
	//	}

	if (turning) {

		///FVector direction = FVector(0, 0, 0) - this->GetActorLocation();
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Turning");

		FVector direction = this->GetActorLocation()*-1;
		this->SetActorRotation(FQuat::Slerp(this->GetActorRotation().Quaternion(), direction.ToOrientationQuat(), 1000.0f));
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

void ATestBasicAI::SetGoal(tuple<int, FVector, int, string> newGoal) {

	currentGoal = newGoal;
	hasGoal = true;



}

void ATestBasicAI::GetController() {


}


void ATestBasicAI::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
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





	// if (!hasGoal) {

	vector<ATestBasicAI*> botVector;
	for (TActorIterator<ATestAIController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{




		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		//currentGoal = ActorItr->GetNewGoal();
		//goalPos = ActorItr->GoalPosition;
		botVector = ActorItr->bots;
		mergePolicey = ActorItr->mergePolicey;

		//hasGoal = true;

	}

	//}



	   // Will need one loop for avoidance vector
	   // will need another loop for centre and average speed. 


	FVector nogroupvcentre(0, 0, 0);

	float nogroupgSpeed = speed;


	int nogroupgroupSize = 0;

	FVector vcentre(0, 0, 0);
	FVector vavoid(0, 0, 0);

	// gSpeed = speed does some crazy shit
	//float gSpeed = 0.1f;
	float gSpeed = speed;
	float dist;

	int groupSize = 0;


	// this will just set the avoidance vector 




	// If the current AI has a group and therefore a goal do the following:
	if (hasGroup) {
		for (TActorIterator<ATestBasicAI> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->GetName() != this->GetName())
			{
				dist = (this->GetActorLocation() - ActorItr->GetActorLocation()).Size();
				if (get<0>(currentGoal) == get<0>(ActorItr->currentGoal)) {
			
					vcentre += ActorItr->GetActorLocation();
					gSpeed = gSpeed + ActorItr->speed;
					groupSize = get<2>(currentGoal);

					// was 2.0f
					// is this bound by size or speeed ? 
					// creating a lower ovidance allows them to swim in cirlces more.
					if (dist <= avoidanceDistance)
					{
						vavoid = vavoid + (this->GetActorLocation() - ActorItr->GetActorLocation());
					}



				}
			}
		}


		vcentre = vcentre / groupSize + (get<1>(currentGoal) - this->GetActorLocation());
		speed = gSpeed / groupSize;

		FVector direction = (vcentre + (vavoid)) - this->GetActorLocation();

		//if (FMath::RandRange(0, 10000) < 100) {
		//	speed = speed * 10;
			// needs to be on a timer.
	//	}

		this->SetActorRotation(FQuat::Slerp(this->GetActorRotation().Quaternion(), direction.ToOrientationQuat(), rotationSpeed * DeltaTime));
		//speed = FMath::FRandRange(speedUpper, speedLower);


		// Get new version of goaal in case instuctions have ghanged
		for (TActorIterator<ATestAIController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{

			tuple<int, FVector, int, string> newGoal = ActorItr->CheckGoal(get<0>(currentGoal));
			
			SetGoal(newGoal);

		}

		// check if the "CANCELLED" instruciton has been set

		if (get<3>(currentGoal) == "CANCELLED") {

			hasGroup = false;
			hasGoal = false;
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, "Cancelled");



		}







	}
	// else we need to assign aa group and goal 
	else {
		TArray<ATestBasicAI*> unGroupedPotentialGroup;
	

		TArray<ATestBasicAI*> groupedPotentialGroup;

		//TArray<ATestBasicAI*> test;

		vector<int> groupedObjectiveID(maxGroupSize);





		int numberGroupsFound = 0;

		int unGroupedGrowingGroupSize = 1;
		int groupedGrowingGroupSize = 0;

	//	for (int i = 0; i < bots.size(); i++) {
		//	ATestBasicAI* ActorItr = bots[i];


		//}

		// try and store iterator for each group? 
		for (TActorIterator<ATestBasicAI> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->GetName() != this->GetName())
			{

				dist = (this->GetActorLocation() - ActorItr->GetActorLocation()).Size();
				//FVector otherGoal = ActorItr->currentGoal;

				// else they have no group

				if (dist <= neighbourDistance) {
					if (ActorItr->hasGroup == false) {
						if (unGroupedGrowingGroupSize < maxGroupSize) {
							
							unGroupedPotentialGroup.Add (*ActorItr);
							//test.Add(*ActorItr);
							
							unGroupedGrowingGroupSize++;
						}
					}
					else {

						if (groupedGrowingGroupSize < maxGroupSize) {
							//	groupedPotentialGroup[groupedGrowingGroupSize] = ActorItr.operator->;

							if (find(groupedObjectiveID.begin(), groupedObjectiveID.end(), get<0>(ActorItr->currentGoal)) != groupedObjectiveID.end()) {
							}
							else {
								groupedObjectiveID[numberGroupsFound] = get<0>(ActorItr->currentGoal);
								
								groupedPotentialGroup.Add(*ActorItr);
								//groupedPotentialGroup[numberGroupsFound] = *ActorItr;
								numberGroupsFound++;
							}
							groupedGrowingGroupSize++;
						}

					}



					//if (dist <= neighbourDistance)
					//	{


					// was 2.0f
					// is this bound by size or speeed ? 
					// creating a lower ovidance allows them to swim in cirlces more.



					//}
				}

			}
		}


		if (!hasGroup) {
			if ((mergePolicey == "CREATE_NEW") && (unGroupedGrowingGroupSize > minGroupSize)) {
				tuple<int, FVector, int, string> newGoal;

				for (TActorIterator<ATestAIController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{

					newGoal = ActorItr->GetNewGoal(unGroupedGrowingGroupSize);
					//newGoal = make_tuple(0, FVector(0, 0, 0), 4);

				}
				if (unGroupedGrowingGroupSize > 1) {
					float groupAvoidance = FMath::RandRange(randomSpacingLower, randomSpacingUpper);
					// -1 as not inlcluding this ai
					for (int i = 0; i < get<2>(newGoal)-1; i++) {
						//test[i]->hasGroup = true;
						//test[i]->SetGoal(newGoal);
						unGroupedPotentialGroup[i]->hasGroup = true;
						unGroupedPotentialGroup[i]->SetGoal(newGoal);

						if (randomSpacing) {
							unGroupedPotentialGroup[i]->avoidanceDistance = groupAvoidance;
						}

					}


					hasGroup = true;
					SetGoal(newGoal);
					if (randomSpacing) {
						avoidanceDistance = groupAvoidance;
					}
				}


			}
			else if ((mergePolicey == "JOIN_EXISTING") || (unGroupedGrowingGroupSize< minGroupSize)) {
				int groupToJoin = -1;
				tuple<int, FVector, int, string> newGoal;
				for (TActorIterator<ATestAIController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{

					newGoal = ActorItr->AssignGroupTojoin(groupedObjectiveID);
				}
				// this is inefficient 		

				SetGoal(newGoal);
				//SetGoal(groupMember->currentGoal);
				hasGroup = true;


			}
		}








	}









	







		// Look for eligible AI- if find not in group
		// might remove this to get more from solo fish.

	}





