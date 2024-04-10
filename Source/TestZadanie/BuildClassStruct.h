// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EBuilding : uint8
{
	None,
	SourceBuild,
	TransmitterBuild,
	PortalBuild,
	Count UMETA(Hidden)
};

struct SBuildInfo
{
	int Resource;
	FVector BuildLocation;
	FRotator BuildRotation;
};

struct SUniqBuild
{
	FString Id;
	FString TargetId;
	EBuilding Type;
	SBuildInfo SavedBuild;
};

struct SLevelInfo
{
	TArray<SUniqBuild> UniqBuild;
};
