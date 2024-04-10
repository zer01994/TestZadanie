// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestZadanieHUD.generated.h"

UCLASS()
class ATestZadanieHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATestZadanieHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

