// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserPointerByChannel.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALaserPointerByChannel::ALaserPointerByChannel(): TraceChannel(ECC_WorldStatic), DefaultResponse(ECR_Block)
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
void ALaserPointerByChannel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserPointerByChannel::Tick(float DeltaTime) {
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


	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Laser_Trace")), bTraceComplex, this);
	TraceParams.bIgnoreBlocks = bIgnoreBlocks;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bIgnoreTouches = bIgnoreTouches;
	TraceParams.bFindInitialOverlaps = bFindInitialOverlaps;

	FCollisionResponseParams ResponseParams = FCollisionResponseParams(DefaultResponse);


	FVector StartTrace = GetActorQuat().RotateVector(FVector(0, 0, 0)) + GetActorLocation();
	FVector EndTrace = GetActorQuat().RotateVector(FVector(10000, 0, 0)) + GetActorLocation();

	if (bMultiHit) {
		TArray<FHitResult> Hits;

		World->LineTraceMultiByChannel(
			Hits, 
			StartTrace,
			EndTrace,
			TraceChannel,
			TraceParams,
			ResponseParams
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
		World->LineTraceSingleByChannel(
			Hit,
			StartTrace,
			EndTrace,
			TraceChannel,
			TraceParams,
			ResponseParams
		);
		if (Hit.bBlockingHit) {
			DrawDebugPoint(World, Hit.Location, 10, FColor(255, 0, 255));
		}
	}
	DrawDebugLine(World, StartTrace, EndTrace, FColor(0, 255, 0));
}

// This ultimately is what controls whether or not it can even tick at all in the editor view port. 
//But, it is EVERY view port so it still needs to be blocked from preview windows and junk.
bool ALaserPointerByChannel::ShouldTickIfViewportsOnly() const {
	return GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor;
}