// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserPointerByObject.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALaserPointerByObject::ALaserPointerByObject(): ObjectQueryType(FCollisionObjectQueryParams::AllObjects)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// add Cylinder to root
	UStaticMeshComponent* Geometry = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	Geometry->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("StaticMesh'/LineTracing/LaserPointer.LaserPointer'"));

	if (CylinderAsset.Succeeded())
	{
		Geometry->SetStaticMesh(CylinderAsset.Object);
		Geometry->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Geometry->SetWorldScale3D(FVector(10.0f));
	}
}

// Called when the game starts or when spawned
void ALaserPointerByObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserPointerByObject::Tick(float DeltaTime) {
	if (GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor)
	{
#if WITH_EDITOR
		BlueprintEditorTick(DeltaTime);
#endif
	} else
	{
		Super::Tick(DeltaTime);
	}

	UWorld* World = GetWorld();


	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ObjectQueryType);
	
	FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Laser_Trace")), bTraceComplex, this);
	Params.bIgnoreBlocks = bIgnoreBlocks;
	Params.bReturnPhysicalMaterial = false;
	Params.bIgnoreTouches = bIgnoreTouches;
	Params.bFindInitialOverlaps = bFindInitialOverlaps;
	

	FVector StartTrace = GetActorQuat().RotateVector(FVector(0, 0, 0)) + GetActorLocation();
	FVector EndTrace = GetActorQuat().RotateVector(FVector(10000, 0, 0)) + GetActorLocation();

	if (bMultiHit) {
		TArray<FHitResult> Hits;

		World->LineTraceMultiByObjectType(
			Hits, 
			StartTrace,
			EndTrace,
			ObjectQueryParams,
			Params
		);
		for (auto Hit : Hits) {
			if (Hit.bBlockingHit) {
				DrawDebugPoint(World, Hit.Location, 10, FColor(255, 0, 255));
			} else {
				DrawDebugPoint(World, Hit.Location, 10, FColor(255, 255, 0));
			}
		}
	} else {
		FHitResult Hit(ForceInit);
		World->LineTraceSingleByObjectType(
			Hit,
			StartTrace,
			EndTrace,
			ObjectQueryParams,
			Params
		);
		if (Hit.bBlockingHit) {
			DrawDebugPoint(World, Hit.Location, 10, FColor(255, 0, 255));
		}
	}
	DrawDebugLine(World, StartTrace, EndTrace, FColor(0, 255, 0));
}

// This ultimately is what controls whether or not it can even tick at all in the editor view port. 
//But, it is EVERY view port so it still needs to be blocked from preview windows and junk.
bool ALaserPointerByObject::ShouldTickIfViewportsOnly() const {
	return GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor;
}