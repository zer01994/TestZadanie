// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SourceBuild.h"
#include "TransmitterBuild.h"
#include "PortalBuild.h"
#include "BuildClassStruct.h"
#include "BuildingSubsystem.generated.h"

UCLASS()
class TESTZADANIE_API UBuildingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void AddBuild(int _CurrentLevel, SUniqBuild _UniqBuild);

	void ChangeLevel(int _CurrentLevel, int _NewLevel);

	TArray<SUniqBuild> LoadBuilds(int _CurrentLevel);

	void UpdateBuilds(int _CurrentLevel, TArray<SUniqBuild> _UpdatedBuilds);

	TArray<SLevelInfo> LoadAllWorld();

	void UpdateAllWorld(TArray<SLevelInfo> _AllWorld);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	TArray<SLevelInfo> AllLevels;
};
