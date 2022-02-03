// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Ballista.generated.h"

/**
 * 
 */
UCLASS()
class MOVESHOOTERMECHANICS_API ABallista : public AWeapon
{
	GENERATED_BODY()

	

public:

	ABallista();

	virtual void Fire() override;

	
};
