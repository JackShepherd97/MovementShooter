// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "VaultComponent.generated.h"

class AMoveShooterMechanicsCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVault);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOVESHOOTERMECHANICS_API UVaultComponent : public UActorComponent
{
	GENERATED_BODY()

	AMoveShooterMechanicsCharacter* PlayerCharacter;

	bool bCanTrace = true;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanVault = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector LedgeLocation = FVector::ZeroVector;

	float TimeToVault = 0.5f;
	
	float CurrentVaultTime = 0;

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve LerpCurve;

	
public:	
	// Sets default values for this component's properties
	UVaultComponent();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FOnVault OnVault;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void VaultTrace();

	void StartVault(float DeltaTime);

	void EndVault();
	
};
