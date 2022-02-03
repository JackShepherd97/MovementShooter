// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoveShooterMechanicsGameMode.h"
#include "MoveShooterMechanicsHUD.h"
#include "MoveShooterMechanicsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMoveShooterMechanicsGameMode::AMoveShooterMechanicsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMoveShooterMechanicsHUD::StaticClass();
}
