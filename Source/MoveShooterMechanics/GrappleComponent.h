// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"

class AMoveShooterMechanicsCharacter;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOVESHOOTERMECHANICS_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

	AMoveShooterMechanicsCharacter* PlayerCharacter;

	UCameraComponent* FirstPersonCameraComponent;

	FVector GrappleLocation = FVector::ZeroVector;

	FVector GrappleNormal = FVector::ZeroVector;

	bool bCanGrapple = false;
	bool bStopMovement = false;

	float DefaultGroundFriction;
	float GrapplingGroundFriction;

	float DefaultGravityScale;
	float GrapplingGravityScale;

	//FVector VelocityBeforeGrapple = FVector::ZeroVector;

	FVector PlayerRightBeforeGrapple = FVector::ZeroVector;

	FTimerHandle GrappleHandle;

public:	
	// Sets default values for this component's properties
	UGrappleComponent();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GrappleLineTrace(FVector CrosshairLocation);

	void UpdatePlayerGrappleState();

	void StartGrapple(float DeltaTime);

	

	

	void EndGrapple();

};
