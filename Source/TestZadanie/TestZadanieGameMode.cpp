// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestZadanieGameMode.h"
#include "TestZadanieHUD.h"
#include "TestZadanieCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestZadanieGameMode::ATestZadanieGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestZadanieHUD::StaticClass();
}
