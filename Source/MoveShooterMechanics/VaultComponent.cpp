// Fill out your copyright notice in the Description page of Project Settings.


#include "VaultComponent.h"
#include "MoveShooterMechanicsCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"






// Sets default values for this component's properties
UVaultComponent::UVaultComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}
/*

// Called when the game starts
void UVaultComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMoveShooterMechanicsCharacter>(GetOwner());
}


// Called every frame
void UVaultComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	VaultTrace();

	StartVault(DeltaTime);
}

void UVaultComponent::VaultTrace()
{
	if (PlayerCharacter->GetCharacterMovement()->IsFalling() && PlayerCharacter->InputAxisForward > 0.f && bCanTrace)
	{
		FVector CapsuleTraceStart = PlayerCharacter->GetActorLocation();
		FVector CapsuleTraceEnd = CapsuleTraceStart + PlayerCharacter->GetActorForwardVector() * 50.f;

		float CapsuleRadius = PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float CapsuleHalfHeight = PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FHitResult FrontOfPlayerResult;

		const TArray<AActor*> ActorsToIgnore
		{
			PlayerCharacter
		};

		UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), CapsuleTraceStart, CapsuleTraceEnd, CapsuleRadius, CapsuleHalfHeight, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::Type::None, FrontOfPlayerResult, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

		FVector FrontImpactPoint{};
		FVector FrontImpactNormal{};

		if (FrontOfPlayerResult.bBlockingHit)
		{
			FrontImpactPoint = FrontOfPlayerResult.ImpactPoint;
			FrontImpactNormal = FrontOfPlayerResult.ImpactNormal;
		}
		else
		{
			bCanVault = false;
			return;
		}

		FVector DownTraceEnd = FVector(FrontImpactPoint.X, FrontImpactPoint.Y, PlayerCharacter->GetActorLocation().Z);
		FVector DownTraceStart = DownTraceEnd;
		DownTraceStart.Z += 100.f;

		FHitResult DownTraceResult;

		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), DownTraceStart, DownTraceEnd, CapsuleRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::Type::None, DownTraceResult, true, FLinearColor::Blue, FLinearColor::Yellow, 0.1f);

		FVector DownTraceImpactPoint{};
		FVector DownTraceImpactNormal{};

		if (DownTraceResult.bBlockingHit)
		{
			DownTraceImpactPoint = DownTraceResult.ImpactPoint;
			DownTraceImpactNormal = DownTraceResult.ImpactNormal;
		}
		else
		{
			bCanVault = false;
			return;
		}

		FVector SpaceForPlayerMiddle = DownTraceImpactPoint;
		SpaceForPlayerMiddle.Z += CapsuleHalfHeight + 1.f;

		FVector SpaceForPlayerTraceStart = SpaceForPlayerMiddle;
		SpaceForPlayerTraceStart.Z -= PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere();

		FVector SpaceForPlayerTraceEnd = SpaceForPlayerMiddle;
		SpaceForPlayerTraceEnd.Z += PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere();

		FHitResult SpaceForPlayerResult;

		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), SpaceForPlayerTraceStart, SpaceForPlayerTraceEnd, CapsuleRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::Type::ForDuration, SpaceForPlayerResult, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

		if (SpaceForPlayerResult.bBlockingHit)
		{
			bCanVault = false;
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Is Vaulting"));
			LedgeLocation = SpaceForPlayerMiddle;
			bCanVault = true;
			PlayerCharacter->SetIsVaulting(true);
			OnVault.Broadcast();
			bCanTrace = false;
			return;
		}
	}

	

}

void UVaultComponent::StartVault(float DeltaTime)
{
	/*
	if (bCanVault)
	{
		if (CurrentVaultTime <= TimeToVault)
		{
			CurrentVaultTime += DeltaTime;
			float TimeRatio = FMath::Clamp(CurrentVaultTime / TimeToVault, 0.f, 1.0f);
			float LerpAlpha = LerpCurve.GetRichCurve()->Eval(TimeRatio);
			FVector NewLocation = UKismetMathLibrary::VLerp(PlayerCharacter->GetActorLocation(), LedgeLocation, LerpAlpha);
			PlayerCharacter->SetActorLocation(NewLocation);

			if (LerpAlpha >= 1.f || CurrentVaultTime >= TimeToVault)
			{
				bCanVault = false;
				CurrentVaultTime = 0;
				PlayerCharacter->SetIsVaulting(false);
			}
		}
	}
	

}*/


