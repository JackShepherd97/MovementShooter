// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoveShooterMechanicsCharacter.h"
#include "MoveShooterMechanicsProjectile.h"
#include "Animation/AnimInstance.h"
#include "Engine/EngineTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GrappleComponent.h"
#include "LinearDashComponent.h"
#include "VaultComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionControllerComponent.h"
#include "Repulsor.h"
#include "WallRunComponent.h"
#include "Weapon.h"
#include "Shotgun.h"
#include "Ballista.h"





DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMoveShooterMechanicsCharacter

AMoveShooterMechanicsCharacter::AMoveShooterMechanicsCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	LinearDashComponent = CreateDefaultSubobject<ULinearDashComponent>(TEXT("Linear Dash Component"));

	GrappleComponent = CreateDefaultSubobject<UGrappleComponent>(TEXT("Grapple Component"));

	VaultComponent = CreateDefaultSubobject<UVaultComponent>(TEXT("Vault Component"));

	WallRunComponent = CreateDefaultSubobject<UWallRunComponent>(TEXT("Wall Run Component"));

	InputAxisForward = 0;
	InputAxisRight = 0;

	W_CurrentWeapon = EWeapon::W_Ballista;
	CurrentWeaponClass = BallistaClass;
}

void AMoveShooterMechanicsCharacter::BeginPlay()
{
	Super::BeginPlay();

	VaultComponent->OnVault.AddDynamic(this, &AMoveShooterMechanicsCharacter::OnVault);

	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(CurrentWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
	CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	Mesh1P->SetHiddenInGame(false, true);
}



void AMoveShooterMechanicsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMoveShooterMechanicsCharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMoveShooterMechanicsCharacter::StartDash);
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &AMoveShooterMechanicsCharacter::ToggleGrapple);
	PlayerInputComponent->BindAction("EndGrapple", IE_Pressed, this, &AMoveShooterMechanicsCharacter::EndGrapple);
	PlayerInputComponent->BindAction("Repulsor", IE_Pressed, this, &AMoveShooterMechanicsCharacter::FireRepulsor);


	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMoveShooterMechanicsCharacter::OnFire);
	PlayerInputComponent->BindAction("Ballista", IE_Pressed, this, &AMoveShooterMechanicsCharacter::EquipBallista);
	PlayerInputComponent->BindAction("Shotgun", IE_Pressed, this, &AMoveShooterMechanicsCharacter::EquipShotgun);



	//PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMoveShooterMechanicsCharacter::FireButtonReleased);


	PlayerInputComponent->BindAxis("MoveForward", this, &AMoveShooterMechanicsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMoveShooterMechanicsCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMoveShooterMechanicsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMoveShooterMechanicsCharacter::LookUpAtRate);
}

void AMoveShooterMechanicsCharacter::OnFire()
{
	CurrentWeapon->Fire();
}

void AMoveShooterMechanicsCharacter::UpdateCurrentWeapon(EWeapon NewWeapon)
{
	CurrentWeapon->Destroy();
	W_CurrentWeapon = NewWeapon;

	switch (W_CurrentWeapon)
	{
		case EWeapon::W_Ballista:
		{
			CurrentWeaponClass = BallistaClass;
			break;
		}
		case EWeapon::W_Shotgun:
		{
			CurrentWeaponClass = ShotgunClass;
			break;
		}
	}

	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(CurrentWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
	CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AMoveShooterMechanicsCharacter::EquipBallista()
{
	if (W_CurrentWeapon != EWeapon::W_Ballista)
	{
		UpdateCurrentWeapon(EWeapon::W_Ballista);
	}
}

void AMoveShooterMechanicsCharacter::EquipShotgun()
{
	if (W_CurrentWeapon != EWeapon::W_Shotgun)
	{
		UpdateCurrentWeapon(EWeapon::W_Shotgun);
	}
}

void AMoveShooterMechanicsCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}

	InputAxisForward = Value;
}

void AMoveShooterMechanicsCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
	
	InputAxisRight = Value;
}

void AMoveShooterMechanicsCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMoveShooterMechanicsCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMoveShooterMechanicsCharacter::Jump()
{
	bJumpRequested = true;
	
	if (JumpCount != 2 && LinearDashComponent->IsDashing() == false && WallRunComponent->IsWallRunning() == false)
	{
		if (JumpCount < 1)
		{
			LaunchCharacter(FVector(0, 0, 600.f), false, true);
		}
		else
		{
			LaunchCharacter(FVector(0, 0, 1000.f), false, true);
		}

		JumpCount++;
	}
	else if (WallRunComponent->IsWallRunning())
	{
		WallRunComponent->CanWallRun(false);
		FVector JumpDirection = WallRunComponent->GetWallNormal();
		FVector JumpDirectionRotated = UKismetMathLibrary::RotateAngleAxis(JumpDirection, 45.f, WallRunComponent->GetWallPlayerCross());

		if (InputAxisRight < 0)
		{
			JumpDirectionRotated += WallRunComponent->GetWallPlayerCross();
		}
		else if (InputAxisRight > 0)
		{
			JumpDirectionRotated += WallRunComponent->GetWallPlayerCrossNegated();
		}

		LaunchCharacter(JumpDirectionRotated * 1000.f, true, true);
		WallRunComponent->StartWallRunResetTimer(0.2f);
	}

}

void AMoveShooterMechanicsCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UE_LOG(LogTemp, Warning, TEXT("Landed"));
	bJumpRequested = false;
	JumpCount = 0;
}

void AMoveShooterMechanicsCharacter::StartDash()
{
	LinearDashComponent->GetDashDirection();
}

void AMoveShooterMechanicsCharacter::ToggleGrapple()
{
	GrappleComponent->GrappleLineTrace(FirstPersonCameraComponent->GetForwardVector());
}

void AMoveShooterMechanicsCharacter::EndGrapple()
{
	GrappleComponent->EndGrapple();
}

void AMoveShooterMechanicsCharacter::FireRepulsor()
{
	bIsRepulsing = true;

	const FRotator SpawnRotation = GetControlRotation();
	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Repulsor = GetWorld()->SpawnActor<ARepulsor>(RepulsorClass, SpawnLocation, SpawnRotation);
	GetWorld()->GetTimerManager().SetTimer(RepulsorHandle, this, &AMoveShooterMechanicsCharacter::DestroyRepulsor, 0.1);
}

void AMoveShooterMechanicsCharacter::DestroyRepulsor()
{
	bIsRepulsing = false;

	Repulsor->Destroy();
}

void AMoveShooterMechanicsCharacter::OnVault()
{
	UE_LOG(LogTemp, Warning, TEXT("Broadcast"));
	PlayerVault.Broadcast();
}






