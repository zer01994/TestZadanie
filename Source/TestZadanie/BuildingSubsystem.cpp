// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSubsystem.h"
#include "BuildGameInstance.h"

void UBuildingSubsystem::AddBuild(int _CurrentLevel, SUniqBuild _UniqBuild)
{
	UBuildGameInstance* GI = Cast<UBuildGameInstance>(GetWorld()->GetGameInstance());
	if (!GI)
		return;
	if (AllLevels.IsValidIndex(_CurrentLevel))
	{
		AllLevels[_CurrentLevel].UniqBuild.Add(_UniqBuild);
	}
	else
	{
		AllLevels.AddDefaulted(1);
		AllLevels[_CurrentLevel].UniqBuild.Add(_UniqBuild);
		//GetWorld()->GetTimerManager().SetTimer(TickTimer, this, &UBuildingSubsystem::DebugFunc, 1.f, true);
	}
}

void UBuildingSubsystem::ChangeLevel(int _CurrentLevel, int _NewLevel)
{
	UBuildGameInstance* GI = Cast<UBuildGameInstance>(GetWorld()->GetGameInstance());
	if (!GI)
		return;
	if (!AllLevels.IsValidIndex(_NewLevel))
	{
		AllLevels.AddDefaulted(1);
		GI->ClearBuilds();
	}
	else
	{
		GI->ClearBuilds();
		TArray<SUniqBuild> TempBuilds = LoadBuilds(_NewLevel);
		for (int i = 0; i < TempBuilds.Num(); i++)
		{
			GI->SpawnBuild(TempBuilds[i]);
		}
	}
}

TArray<SUniqBuild> UBuildingSubsystem::LoadBuilds(int _CurrentLevel)
{
	return AllLevels[_CurrentLevel].UniqBuild;
}

void UBuildingSubsystem::UpdateBuilds(int _CurrentLevel, TArray<SUniqBuild> _UpdatedBuilds)
{
	AllLevels[_CurrentLevel].UniqBuild = _UpdatedBuilds;
}

TArray<SLevelInfo> UBuildingSubsystem::LoadAllWorld()
{
	return AllLevels;
}

void UBuildingSubsystem::UpdateAllWorld(TArray<SLevelInfo> _AllWorld)
{
	AllLevels = _AllWorld;
}

void UBuildingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}
