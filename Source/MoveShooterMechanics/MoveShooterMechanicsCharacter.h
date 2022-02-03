// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoveShooterMechanicsCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class ULinearDashComponent;
class UGrappleComponent;
class UVaultComponent;
class ARepulsor;
class UWallRunComponent;
class AWeapon;
class ABallista;
class AShotgun;

UENUM() 
enum class EAbilityState 
{
	AS_None,
	AS_Repulsor,
	AS_Dashing, 
	AS_Blocking,
	AS_Grappling,
};

UENUM()
enum class EMovementState
{
	MST_Grounded,
	MST_Falling,
	MST_Sliding,
	MST_Vaulting
};

UENUM(BlueprintType)
enum class EWeapon : uint8
{
	W_Ballista,
	W_Shotgun,
	W_AR
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerVault);

UCLASS(config=Game)
class AMoveShooterMechanicsCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash Component", meta = (AllowPrivateAccess = "true"))
	ULinearDashComponent* LinearDashComponent;

	UPROPERTY(EditAnywhere)
	UGrappleComponent* GrappleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vault Component", meta = (AllowPrivateAccess = "true"))
	UVaultComponent* VaultComponent;

	UPROPERTY(EditAnywhere)
	UWallRunComponent* WallRunComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARepulsor> RepulsorClass;
	ARepulsor* Repulsor;
	FTimerHandle RepulsorHandle;

	int JumpCount = 0;
	bool bJumpRequested = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsRepulsing = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsVaulting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsWallRunning;

	AWeapon* CurrentWeapon;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> BallistaClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> ShotgunClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> CurrentWeaponClass;

	EWeapon W_CurrentWeapon;

	
	

public:
	AMoveShooterMechanicsCharacter();

	float InputAxisForward;
	float InputAxisRight;

	UPROPERTY(BlueprintAssignable)
	FPlayerVault PlayerVault;

protected:
	virtual void BeginPlay();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AMoveShooterMechanicsProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	FTimerHandle WallRunResetHandle;

protected:

	void Jump();

	virtual void Landed(const FHitResult& Hit) override;


	void StartDash();

	void ToggleGrapple();

	void EndGrapple();

	void OnFire();

	//UFUNCTION()
	//void OnVault();

	void NextWeapon();

	void PreviousWeapon();

	void UpdateCurrentWeapon(EWeapon NewWeapon);

	void EquipBallista();

	void EquipShotgun();

	void FireRepulsor();

	void DestroyRepulsor();

	
	void MoveForward(float Val);

	
	void MoveRight(float Val);

	
	void TurnAtRate(float Rate);

	
	void LookUpAtRate(float Rate);

	
	
	
protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

	
	

public:
	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	bool GetJumpRequested() { return bJumpRequested; }

	UFUNCTION(BlueprintCallable)
	void SetIsVaulting(bool NewBool) { bIsVaulting = NewBool; }

	

	
};

