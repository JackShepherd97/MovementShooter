// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallRunComponent.generated.h"

class AMoveShooterMechanicsCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOVESHOOTERMECHANICS_API UWallRunComponent : public UActorComponent
{
	GENERATED_BODY()

	AMoveShooterMechanicsCharacter* PlayerCharacter;

	bool bCanWallRun = true;
	bool bIsWallRunning = false;

	FVector WallNormal = FVector::ZeroVector;

	FVector WallPlayerCross = FVector::ZeroVector;
	FVector WallPlayerCrossNegated = FVector::ZeroVector;

	FTimerHandle WallRunReset;

public:	
	// Sets default values for this component's properties
	UWallRunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TraceForWall(float DeltaTime);

	void StartWallRun();

	void EndWallRun();

	bool IsWallRunning() { return bIsWallRunning; }

	FVector GetWallNormal() { return WallNormal; }

	FVector GetWallPlayerCross() { return WallPlayerCross; }
	FVector GetWallPlayerCrossNegated() { return WallPlayerCrossNegated; }

	void CanWallRun(bool NewBool) { bCanWallRun = NewBool; }

	void StartWallRunResetTimer(float ResetTime);

	void ResetWallRun();
	

	
};
