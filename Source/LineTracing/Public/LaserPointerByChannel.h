// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserPointerByChannel.generated.h"

UCLASS()
class LINETRACING_API ALaserPointerByChannel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserPointerByChannel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual bool ShouldTickIfViewportsOnly() const override;

	UFUNCTION(BlueprintImplementableEvent, CallInEditor, Category = "Events")
	void BlueprintEditorTick(float DeltaTime);

	UPROPERTY(EditAnywhere)
	bool bMultiHit;
	
	UPROPERTY(EditAnywhere)
	bool bTraceComplex;

	UPROPERTY(EditAnywhere)
	bool bFindInitialOverlaps;

	UPROPERTY(EditAnywhere)
	bool bIgnoreBlocks;

	UPROPERTY(EditAnywhere)
	bool bIgnoreTouches;

	UPROPERTY(EditAnywhere)
	TEnumAsByte <ECollisionResponse> DefaultResponse;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
