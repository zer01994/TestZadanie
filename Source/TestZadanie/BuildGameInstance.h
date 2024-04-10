// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BuildingSubsystem.h"
#include "BuildGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTZADANIE_API UBuildGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void AddBuild(int _CurrentLevel, SUniqBuild _UniqBuild);

	void ChangeLevel(int _CurrentLevel, int _NewLevel);

	int CurrentLevel = 0;
	
	virtual void Init() override;

	void SpawnBuild(SUniqBuild _UniqBuild);

	void ClearBuilds();

	void CheckTargetBuild();

	TArray<ABuildingParent*> SavedBuilds;

	FString GetTargetId(FString _Id);

	UPROPERTY(Category = Build, EditAnywhere, BlueprintReadOnly)
	int PortalResource = 0;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Build)
	TSubclassOf<class ASourceBuild> SourceBuild;

	UPROPERTY(EditDefaultsOnly, Category = Build)
	TSubclassOf<class ATransmitterBuild> TransmitterBuild;

	UPROPERTY(EditDefaultsOnly, Category = Build)
	TSubclassOf<class APortalBuild> PortalBuild;

	FTimerHandle TickTimerHandle;

	void DebugFunc();

	void TickTimer();

	bool CheckTargetResource(FString _TargetId, TArray<SLevelInfo> _AllWorlds);

	bool CheckTargetConflict(FString _TargetId, FString _CheckId);

};
