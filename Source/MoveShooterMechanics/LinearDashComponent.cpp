// Fill out your copyright notice in the Description page of Project Settings.


#include "LinearDashComponent.h"
#include "MoveShooterMechanicsCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
ULinearDashComponent::ULinearDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void ULinearDashComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMoveShooterMechanicsCharacter>(GetOwner());
	
}


// Called every frame
void ULinearDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	StartDash(DeltaTime);
	DashLedgeAndGroundTrace();
}

void ULinearDashComponent::GetDashDirection()
{
	FVector GroundTraceEnd = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorUpVector() * -150.f;

	FHitResult GroundTraceHit;

	//Line trace from the actors position to the ground
	GetWorld()->LineTraceSingleByChannel(GroundTraceHit, PlayerCharacter->GetActorLocation(), GroundTraceEnd, ECollisionChannel::ECC_Visibility);
	DrawDebugLine(GetWorld(), PlayerCharacter->GetActorLocation(), GroundTraceEnd, FColor::Red, false, 2.0f, 2.0f, 2.0f);
	DrawDebugSphere(GetWorld(), GroundTraceEnd, 36.f, 12.f, FColor::Red, false, 2.0f, 2.0f, 2.0f);

	YawBeforeDash = ActorYaw();

	if (GroundTraceHit.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), GroundTraceHit.ImpactPoint, 36.f, 12.f, FColor::Red, false, 0.1f, 5.0f, 5.0f);
		//Getting the cross product of the ground impact normal and our actors right vector gives us an vector facing behind the player, we will negate this to make it face forward
		FVector GroundActorCross = UKismetMathLibrary::Cross_VectorVector(GroundTraceHit.ImpactNormal, PlayerCharacter->GetActorRightVector());
		FVector GroundActorCrossNegated = UKismetMathLibrary::NegateVector(GroundActorCross);
		UKismetMathLibrary::Vector_Normalize(GroundActorCrossNegated);

		DashStartPoint = PlayerCharacter->GetActorLocation();
		DashDirection = GroundActorCrossNegated;

		//If any input values are detected then we will rotate our direction vector by the our actors yaw, else the default is our forward facing vector
		if(PlayerCharacter->InputAxisForward != 0 || PlayerCharacter->InputAxisRight != 0)
		{
			DashDirection = UKismetMathLibrary::RotateAngleAxis(GroundActorCrossNegated, YawBeforeDash, PlayerCharacter->GetActorUpVector());
		}

		PlayerCharacter->GetMovementComponent()->SetPlaneConstraintEnabled(true);
		PlayerCharacter->GetMovementComponent()->SetPlaneConstraintFromVectors(DashDirection, PlayerCharacter->GetActorUpVector());
		PlayerCharacter->GetCharacterMovement()->BrakingFriction = 100.f;
		
		bIsDashing = true;

	}
	else
	{
		DashDirection = PlayerCharacter->GetActorForwardVector();

		if (PlayerCharacter->InputAxisForward != 0 || PlayerCharacter->InputAxisRight != 0)
		{
			DashDirection = UKismetMathLibrary::RotateAngleAxis(PlayerCharacter->GetActorForwardVector(), YawBeforeDash, PlayerCharacter->GetActorUpVector());
		}
		
		
		bIsAirDashing = true;
	}

	UKismetMathLibrary::Vector_Normalize(DashDirection);
	PlayerCharacter->LaunchCharacter(DashDirection * 100.f, true, true); //velocity change that immediately overrides our current velocity, as we cant do so in tick

	StartDashTrace = true;
}

void ULinearDashComponent::StartDash(float DeltaTime)
{
	if (bIsDashing)
	{
		
		FVector StartToCurrentLocation = PlayerCharacter->GetActorLocation() - DashStartPoint;
		float Size = StartToCurrentLocation.Size();
			
		PlayerCharacter->LaunchCharacter(DashDirection * GroundedDashSpeed * DeltaTime, false, false);

			

			if (Size >= DashLength)
			{
				UE_LOG(LogTemp, Warning, TEXT("%f"), Size);
				
				
				EndDash();
				
			}

			
		
	}
	else if (bIsAirDashing)
	{
		FVector StartToCurrentLocation = PlayerCharacter->GetActorLocation() - DashStartPoint;
		float Size = StartToCurrentLocation.Size();
			
		PlayerCharacter->LaunchCharacter(DashDirection * AirDashSpeed * DeltaTime, false, false);

		if (Size >= DashLength)
		{
			PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();


			EndDash();
				
		}
		
	}
}

void ULinearDashComponent::DashLedgeAndGroundTrace()
{
	if (StartDashTrace)
	{
		FVector DashGroundTraceEnd = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorUpVector() * -1000.f;

		FHitResult DashGroundTraceHit;

		GetWorld()->LineTraceSingleByChannel(DashGroundTraceHit, PlayerCharacter->GetActorLocation(), DashGroundTraceEnd, ECollisionChannel::ECC_Visibility);

		if (DashGroundTraceHit.bBlockingHit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Ground Hit"));

			TArray<AActor*> ActorsToIgnore
			{
				PlayerCharacter
			};

			FVector DashDirectionStart = PlayerCharacter->GetActorLocation();
			FVector DashDirectionEnd = DashDirectionStart + DashDirection * 50.f;

			FHitResult DashDirectionTrace;

			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(PlayerCharacter);

			//Trace in direction we want to go in (actor half-height)
			UKismetSystemLibrary::SphereTraceSingle(this, DashDirectionStart, DashDirectionEnd, 36.f, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::Type::None, DashDirectionTrace, true, FLinearColor::Blue, FLinearColor::Yellow, 3.0f);


			if (DashDirectionTrace.bBlockingHit)
			{
				DashDirectionEnd = DashDirectionTrace.ImpactPoint;
			}

			FVector DashDirectionTraceReversedStart = DashDirectionEnd;
			DashDirectionTraceReversedStart.Z -= PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

			FVector DashDirectionTraceReversedEnd = DashDirectionStart;
			DashDirectionTraceReversedEnd.Z -= PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

			FHitResult DashDirectionTraceReversed;

			UKismetSystemLibrary::SphereTraceSingle(this, DashDirectionTraceReversedStart, DashDirectionTraceReversedEnd, 20.f, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::Type::ForDuration, DashDirectionTraceReversed, true, FLinearColor::Red, FLinearColor::Green, 3.0f);

			if (DashDirectionTraceReversed.bBlockingHit)
			{
				//If the impact normal is less than this value, it means we are on the edge of a ledge
				if (DashDirectionTraceReversed.ImpactNormal.Z < 0.3f)
				{
					if (PlayerCharacter->InputAxisForward == 0 && PlayerCharacter->InputAxisRight == 0)
					{
						StartDashTrace = false;
						FVector VelocityBeforeStop = PlayerCharacter->GetVelocity();

						PlayerCharacter->GetCharacterMovement()->GroundFriction = 8.0f;
						PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
						EndDash();
						//UE_LOG(LogTemp, Warning, TEXT("Ledge Stop"));
					}
				}
			}
		}
	}
}

void ULinearDashComponent::EndDash()
{
	bIsDashing = false;
	bIsAirDashing = false;

	PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
	PlayerCharacter->GetMovementComponent()->SetPlaneConstraintEnabled(false);
	GetWorld()->GetTimerManager().SetTimer(DashHandle, this, &ULinearDashComponent::EndDashTrace, 0.5);
}

void ULinearDashComponent::EndDashTrace()
{
	StartDashTrace = false;
}

float ULinearDashComponent::ActorYaw()
{
	//Our characters velocity "rotation" and aim rotation will give us a value we can use for the direction of our dash
	FRotator AccelerationRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration());
	FRotator ControlRotation = PlayerCharacter->GetControlRotation();
	float MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(AccelerationRotation, ControlRotation).Yaw;
	return MovementOffsetYaw;
}


