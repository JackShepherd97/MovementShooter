// Fill out your copyright notice in the Description page of Project Settings.


#include "Ballista.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"




ABallista::ABallista()
{
	MaxAmmo = 1;
	AmmoCount = 0;
	TimeToReload = 2.f;
}

void ABallista::Fire()
{
	if (AmmoCount < MaxAmmo)
	{
		bIsFiring = true;

		const FTransform MuzzleTransform = GetMuzzleComponent()->GetComponentTransform();
		FVector MuzzleLocation = MuzzleTransform.GetLocation();
		FQuat MuzzleQuat = MuzzleTransform.GetRotation();
		FRotator MuzzleRotation = MuzzleQuat.Rotator();
		FVector MuzzleDirection = UKismetMathLibrary::GetForwardVector(MuzzleRotation);
		FVector LineTraceEnd = MuzzleLocation + MuzzleDirection * 500000.f;
		FVector EndPoint = LineTraceEnd;

		FHitResult BallistaResult;

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, GetMuzzleComponent()->GetComponentTransform());
		}

		GetWorld()->LineTraceSingleByChannel(BallistaResult, MuzzleLocation, LineTraceEnd, ECollisionChannel::ECC_Visibility);

		if (BallistaResult.bBlockingHit)
		{
			EndPoint = BallistaResult.ImpactPoint;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, EndPoint);
		}

		if (BulletTrace)
		{
			UParticleSystemComponent* BulletTraceStart = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTrace, GetMuzzleComponent()->GetComponentTransform());

			if (BulletTraceStart)
			{
				BulletTraceStart->SetVectorParameter(FName("Target"), EndPoint);
			}
		}

		AmmoCount++;
		StartReload();
	}
}