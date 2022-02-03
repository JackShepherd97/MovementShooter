// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MoveShooterMechanicsHUD.generated.h"

UCLASS()
class AMoveShooterMechanicsHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMoveShooterMechanicsHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

