// Fill out your copyright notice in the Description page of Project Settings.


#include "WallRunComponent.h"
#include "MoveShooterMechanicsCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"





// Sets default values for this component's properties
UWallRunComponent::UWallRunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWallRunComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMoveShooterMechanicsCharacter>(GetOwner());
	
}


// Called every frame
void UWallRunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceForWall(DeltaTime);
}

void UWallRunComponent::TraceForWall(float DeltaTime)
{
	TArray<AActor*> ActorsToIgnore
	{
		PlayerCharacter
	};

	if (PlayerCharacter->InputAxisRight < 0 && bCanWallRun)
	{
		FVector ActorForward = PlayerCharacter->GetActorForwardVector();
		FVector ActorForwardRotated = UKismetMathLibrary::RotateAngleAxis(ActorForward, -45.f, PlayerCharacter->GetActorUpVector());
		FVector Start = PlayerCharacter->GetActorLocation() + ActorForwardRotated * 45.f;

		FVector End = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * -100.f;

		FHitResult LeftWallResult;

		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 36.f, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, LeftWallResult, true, FLinearColor::Red, FLinearColor::Green, 0.1);

		if (LeftWallResult.bBlockingHit)
		{
			WallNormal = LeftWallResult.ImpactNormal;

			FVector BackTraceStart = LeftWallResult.ImpactPoint;
			FVector WallNormalRotated = UKismetMathLibrary::RotateAngleAxis(WallNormal, -90.f, PlayerCharacter->GetActorUpVector());
			UKismetMathLibrary::Vector_Normalize(WallNormalRotated);
			FVector BackTraceEnd = BackTraceStart + WallNormalRotated * -100.f;

			FHitResult BackResult;

			UKismetSystemLibrary::SphereTraceSingle(GetWorld(), BackTraceStart, BackTraceEnd, 36.f, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, BackResult , true, FLinearColor::Red, FLinearColor::Green, 0.1);

			DrawDebugLine(GetWorld(), BackTraceStart, BackTraceEnd, FColor::Red, false, 0.1f, 10.f, 10.f);

			if (WallNormal.Z < 0.5)
			{
				bIsWallRunning = true;
				FVector PlayerToWall =  PlayerCharacter->GetActorLocation() - WallNormal;
				float PlayerToWallSize = PlayerToWall.Size();
				FVector StickToWall = WallNormal * -PlayerToWallSize;
				PlayerCharacter->LaunchCharacter(StickToWall, true, true);

				WallPlayerCross = UKismetMathLibrary::Cross_VectorVector(WallNormal, FVector(0, 0, 1)); 
				UKismetMathLibrary::Vector_Normalize(WallPlayerCross);
				FVector LaunchVector = WallPlayerCross * 1000.f;
				PlayerCharacter->LaunchCharacter(LaunchVector * DeltaTime, false, true);
			}
			
		}
	}
	else if (PlayerCharacter->InputAxisRight > 0 && bCanWallRun)
	{
		FVector ActorForward = PlayerCharacter->GetActorForwardVector();
		FVector ActorForwardRotated = UKismetMathLibrary::RotateAngleAxis(ActorForward, 45.f, PlayerCharacter->GetActorUpVector());
		FVector Start = PlayerCharacter->GetActorLocation() + ActorForwardRotated * 45.f;

		FVector End = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * -100.f;

		FHitResult RightWallResult;

		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 36.f, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, RightWallResult, true, FLinearColor::Red, FLinearColor::Green, 0.1);

		if (RightWallResult.bBlockingHit)
		{
			WallNormal = RightWallResult.ImpactNormal;

			if (WallNormal.Z < 0.5)
			{
				bIsWallRunning = true;
				FVector PlayerToWall = PlayerCharacter->GetActorLocation() - WallNormal;
				float PlayerToWallSize = PlayerToWall.Size();
				FVector StickToWall = WallNormal * -PlayerToWallSize;
				PlayerCharacter->LaunchCharacter(StickToWall, true, true);

				WallPlayerCross = UKismetMathLibrary::Cross_VectorVector(WallNormal, FVector(0, 0, 1));
				WallPlayerCrossNegated = UKismetMathLibrary::NegateVector(WallPlayerCross);
				UKismetMathLibrary::Vector_Normalize(WallPlayerCrossNegated);
				FVector LaunchVector = WallPlayerCrossNegated  * 1000.f;
				PlayerCharacter->LaunchCharacter(LaunchVector * DeltaTime, false, true);
			}
		}
	}
	else
	{
		bIsWallRunning = false;
	}
	
}

void UWallRunComponent::StartWallRunResetTimer(float ResetTime)
{
	GetWorld()->GetTimerManager().SetTimer(WallRunReset, this, &UWallRunComponent::ResetWallRun, ResetTime);
}

void UWallRunComponent::ResetWallRun()
{
	bCanWallRun = true;
}



