// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"




// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SetRootComponent(WeaponMesh);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	Muzzle->SetupAttachment(WeaponMesh);

	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AWeapon::Fire()
{

}

void AWeapon::StartReload()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &AWeapon::EndReload, TimeToReload);
}

void AWeapon::EndReload()
{
	AmmoCount = 0;
}