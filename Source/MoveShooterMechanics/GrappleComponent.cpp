// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MoveShooterMechanicsCharacter.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"



// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMoveShooterMechanicsCharacter>(GetOwner());
	FirstPersonCameraComponent = PlayerCharacter->GetFirstPersonCameraComponent();

	DefaultGroundFriction = PlayerCharacter->GetCharacterMovement()->GroundFriction;
	GrapplingGroundFriction = 0.f;

	DefaultGravityScale = PlayerCharacter->GetCharacterMovement()->GravityScale;
	GrapplingGravityScale = 0.025;
}


// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	StartGrapple(DeltaTime);

	
}

void UGrappleComponent::GrappleLineTrace(FVector CrosshairLocation)
{
	FVector LineTraceStart = PlayerCharacter->GetActorLocation();
	FVector LineTraceEnd = LineTraceStart + CrosshairLocation * 2500.f;

	FHitResult LineTraceResult;

	GetWorld()->LineTraceSingleByChannel(LineTraceResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Visibility);

	if (LineTraceResult.bBlockingHit)
	{
		
		GrappleLocation = LineTraceResult.ImpactPoint;
		GrappleNormal = LineTraceResult.ImpactNormal;

		
		
		UpdatePlayerGrappleState();
	}
}

void UGrappleComponent::UpdatePlayerGrappleState()
{
	PlayerCharacter->LaunchCharacter(FVector(0.f, 0.f, 100.f), false, true);
	PlayerCharacter->GetCharacterMovement()->GravityScale = GrapplingGravityScale;
	PlayerCharacter->GetCharacterMovement()->GroundFriction = GrapplingGravityScale;
	
	PlayerRightBeforeGrapple = PlayerCharacter->GetActorRightVector();
	UKismetMathLibrary::Vector_Normalize(PlayerRightBeforeGrapple);

	FVector VelocityBeforeGrapple = PlayerCharacter->GetVelocity();
	UE_LOG(LogTemp, Warning, TEXT("%f"), PlayerCharacter->InputAxisRight);
	
	if (PlayerCharacter->InputAxisRight == 0)
	{
		PlayerCharacter->LaunchCharacter(PlayerCharacter->GetVelocity() * 0.2f, true, true);

	}
	else
	{
		PlayerCharacter->LaunchCharacter(PlayerCharacter->GetVelocity() * 0.5, true, true);
	}

	

	bCanGrapple = true;
}


void UGrappleComponent::StartGrapple(float DeltaTime)
{
	if (bCanGrapple)
	{
		FVector GrappleToActorVector = GrappleLocation - PlayerCharacter->GetActorLocation();
		float GrappleToActorSize = GrappleToActorVector.Size();
		
		UKismetMathLibrary::Vector_Normalize(GrappleToActorVector);

		FVector DirectionWithCamera = GrappleToActorVector + FirstPersonCameraComponent->GetForwardVector();

		if (PlayerCharacter->InputAxisRight > 0)
		{
			FVector PlayerRightRotated = UKismetMathLibrary::RotateAngleAxis(PlayerRightBeforeGrapple, -45.f, PlayerCharacter->GetActorUpVector());
			DirectionWithCamera += PlayerRightRotated;
		}
		else if (PlayerCharacter->InputAxisRight < 0)
		{
			FVector PlayerRightNegated = PlayerRightBeforeGrapple * -1.0f;
			FVector PlayerRightNegatedRotated = UKismetMathLibrary::RotateAngleAxis(PlayerRightNegated, 45.f, PlayerCharacter->GetActorUpVector());
			DirectionWithCamera += PlayerRightNegatedRotated;
		}
		

		float GrappleToActorDot = UKismetMathLibrary::Dot_VectorVector(GrappleNormal, GrappleToActorVector);

		PlayerCharacter->LaunchCharacter(DirectionWithCamera * 750.f * DeltaTime, false, false);

		UE_LOG(LogTemp, Warning, TEXT("%f"), GrappleToActorDot);

		if (GrappleToActorSize < 200.f || GrappleToActorDot > 0.f)
		{
			EndGrapple();
		}

		
	}
	

}

void UGrappleComponent::EndGrapple()
{
	bCanGrapple = false;
	PlayerCharacter->GetCharacterMovement()->GravityScale = DefaultGravityScale;
	PlayerCharacter->GetCharacterMovement()->GroundFriction = DefaultGroundFriction;
}