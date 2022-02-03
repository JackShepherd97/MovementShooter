// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UParticleSystem;

UCLASS()
class MOVESHOOTERMECHANICS_API AWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* Muzzle;
	
	

public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned


	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	UParticleSystem* BulletTrace;
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int AmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int BulletsPerMinute;

	float TimeToReload;

	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFiring;

	FTimerHandle ReloadHandle;

	virtual void BeginPlay() override;

	

	void StartReload();

	void EndReload();

	USceneComponent* GetMuzzleComponent() { return Muzzle; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Fire();
};
