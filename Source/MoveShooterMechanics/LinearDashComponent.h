// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LinearDashComponent.generated.h"


class AMoveShooterMechanicsCharacter;
class UCameraComponent;
class USkeletalMeshComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOVESHOOTERMECHANICS_API ULinearDashComponent : public UActorComponent
{
	GENERATED_BODY()

	AMoveShooterMechanicsCharacter* PlayerCharacter;

	FTimerHandle DashHandle;
	FTimerHandle DashTraceHandle;
	
	bool bIsDashing = false;
	bool bIsAirDashing = false;
	bool StartDashTrace = false;
	
	FVector DashStartPoint = FVector::ZeroVector;
	FVector DashDirection = FVector::ZeroVector;

	float YawBeforeDash = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Component", meta = (AllowPrivateAccess = "true"))
	float GroundedDashSpeed = 50000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Component", meta = (AllowPrivateAccess = "true"))
	float AirDashSpeed = 175000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Component", meta = (AllowPrivateAccess = "true"))
	float DashLength = 200.f;

public:	
	// Sets default values for this component's properties
	ULinearDashComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	void GetDashDirection();
	void StartDash(float DeltaTime);
	void DashLedgeAndGroundTrace();
	void EndDash();
	void EndDashTrace();
	

	float ActorYaw();

	bool IsDashing() { return bIsDashing; }
};
