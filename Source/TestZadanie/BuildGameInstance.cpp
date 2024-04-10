// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildGameInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UBuildGameInstance::AddBuild(int _CurrentLevel, SUniqBuild _UniqBuild)
{
	UBuildingSubsystem* BuildSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (BuildSubsystem)
		BuildSubsystem->AddBuild(_CurrentLevel, _UniqBuild);
}

void UBuildGameInstance::ChangeLevel(int _CurrentLevel, int _NewLevel)
{
	UBuildingSubsystem* BuildSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (BuildSubsystem)
	{
		BuildSubsystem->ChangeLevel(_CurrentLevel, _NewLevel);
		CurrentLevel = _NewLevel;
	}
}

void UBuildGameInstance::Init()
{
	Super::Init();
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UBuildGameInstance::TickTimer, 1.f, true);
}

void UBuildGameInstance::SpawnBuild(SUniqBuild _UniqBuild)
{
	UWorld* const World = GetWorld();
	if (!World)
		return;
	const FVector SpawnVector = _UniqBuild.SavedBuild.BuildLocation;
	const FRotator SpawnRotation = _UniqBuild.SavedBuild.BuildRotation;
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABuildingParent* SpawnActor;
	switch (_UniqBuild.Type)
	{
	case EBuilding::SourceBuild:
		if (SourceBuild != nullptr)
		{
			SpawnActor = World->SpawnActor<ABuildingParent>(SourceBuild, SpawnVector, SpawnRotation, ActorSpawnParams);
			SpawnActor->LoadBuild = true;
			SpawnActor->Resource = _UniqBuild.SavedBuild.Resource;
			SpawnActor->Id = _UniqBuild.Id;
			SpawnActor->TargetId = _UniqBuild.TargetId;
			SpawnActor->InitParams();
			SavedBuilds.Add(SpawnActor);
		}
		break;

	case EBuilding::TransmitterBuild:
		if (TransmitterBuild != nullptr)
		{
			SpawnActor = World->SpawnActor<ABuildingParent>(TransmitterBuild, SpawnVector, SpawnRotation, ActorSpawnParams);
			SpawnActor->LoadBuild = true;
			SpawnActor->Resource = _UniqBuild.SavedBuild.Resource;
			SpawnActor->Id = _UniqBuild.Id;
			SpawnActor->TargetId = _UniqBuild.TargetId;
			SpawnActor->InitParams();
			SavedBuilds.Add(SpawnActor);
		}
		break;

	case EBuilding::PortalBuild:
		if (PortalBuild != nullptr)
		{
			SpawnActor = World->SpawnActor<ABuildingParent>(PortalBuild, SpawnVector, SpawnRotation, ActorSpawnParams);
			SpawnActor->LoadBuild = true;
			SpawnActor->Resource = _UniqBuild.SavedBuild.Resource;
			SpawnActor->Id = _UniqBuild.Id;
			SpawnActor->TargetId = _UniqBuild.TargetId;
			SpawnActor->InitParams();
			SavedBuilds.Add(SpawnActor);
		}
		break;

	default: break;
	}

}

void UBuildGameInstance::ClearBuilds()
{
	for (int i = 0; i < SavedBuilds.Num(); i++)
	{
		SavedBuilds[i]->Destroy();
	}
	SavedBuilds.Empty();
}

void UBuildGameInstance::DebugFunc()
{
	//UE_LOG(LogTemp, Warning, TEXT("Level"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Current Level: %d"), CurrentLevel));
}

bool UBuildGameInstance::CheckTargetResource(FString _TargetId, TArray<SLevelInfo> _AllWorlds)
{
	bool TempBool = false;
	TArray<SLevelInfo> AllWorlds = _AllWorlds;
	for (int i = 0; i < AllWorlds.Num(); i++)
	{
		for (int j = 0; j < AllWorlds[i].UniqBuild.Num(); j++)
		{
			if (AllWorlds[i].UniqBuild[j].Id == _TargetId)
			{
				if (AllWorlds[i].UniqBuild[j].Type == EBuilding::PortalBuild)
				{
					TempBool = PortalResource > 0;
				}
				else
				{
					TempBool = AllWorlds[i].UniqBuild[j].SavedBuild.Resource > 0;
				}
			}
		}
	}
	return TempBool;
}

bool UBuildGameInstance::CheckTargetConflict(FString _TargetId, FString _CheckId)
{
	bool TempBool = false;
	UBuildingSubsystem* BuildSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (BuildSubsystem)
	{
		TArray<SLevelInfo> AllWorlds = BuildSubsystem->LoadAllWorld();
		for (int i = 0; i < AllWorlds.Num(); i++)
		{
			for (int j = 0; j < AllWorlds[i].UniqBuild.Num(); j++)
			{
				if (AllWorlds[i].UniqBuild[j].Id == _TargetId)
				{
					TempBool = AllWorlds[i].UniqBuild[j].TargetId == _CheckId;
				}
			}
		}
	}
	return TempBool;
}

void UBuildGameInstance::TickTimer()
{
	UBuildingSubsystem* BuildSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (BuildSubsystem)
	{
		TArray<SLevelInfo> AllWorlds = BuildSubsystem->LoadAllWorld();
		TArray<FString> IncreaseResBuilds;
		TArray<FString> DecreaseResBuilds;
		for (int i = 0; i < AllWorlds.Num(); i++)
		{
			for (int j = 0; j < AllWorlds[i].UniqBuild.Num(); j++)
			{
				if (!AllWorlds[i].UniqBuild[j].TargetId.IsEmpty() && AllWorlds[i].UniqBuild[j].SavedBuild.Resource < 100 && CheckTargetResource(AllWorlds[i].UniqBuild[j].TargetId, AllWorlds) && !CheckTargetConflict(AllWorlds[i].UniqBuild[j].TargetId, AllWorlds[i].UniqBuild[j].Id))
				{
					IncreaseResBuilds.Add(AllWorlds[i].UniqBuild[j].Id);
					DecreaseResBuilds.Add(AllWorlds[i].UniqBuild[j].TargetId);
				}
			}
		}
		for (int i = 0; i < AllWorlds.Num(); i++)
		{
			for (int j = 0; j < AllWorlds[i].UniqBuild.Num(); j++)
			{
				for (int p = 0; p < DecreaseResBuilds.Num(); p++)
				{
					if (DecreaseResBuilds[p] == AllWorlds[i].UniqBuild[j].Id)
					{
						if (AllWorlds[i].UniqBuild[j].Type == EBuilding::PortalBuild)
						{
							if (PortalResource > 0)
							{
								PortalResource--;
								for (int t = 0; t < SavedBuilds.Num(); t++)
								{
									if (SavedBuilds.IsValidIndex(t))
									{
										if (SavedBuilds[t]->Id == AllWorlds[i].UniqBuild[j].Id)
										{
											SavedBuilds[t]->OnResourceUpdate(PortalResource);
										}
									}
								}
							}
							else
							{
								IncreaseResBuilds[p] = "";
							}
						}
						else
						{
							if (AllWorlds[i].UniqBuild[j].SavedBuild.Resource > 0)
							{
								AllWorlds[i].UniqBuild[j].SavedBuild.Resource--;
								for (int t = 0; t < SavedBuilds.Num(); t++)
								{
									if (SavedBuilds.IsValidIndex(t))
									{
										if (SavedBuilds[t]->Id == AllWorlds[i].UniqBuild[j].Id)
										{
											SavedBuilds[t]->OnResourceUpdate(AllWorlds[i].UniqBuild[j].SavedBuild.Resource);
										}
									}
								}
							}
							else
							{
								IncreaseResBuilds[p] = "";
							}
						}
					}
				}
				for (int p = 0; p < IncreaseResBuilds.Num(); p++)
				{
					if (IncreaseResBuilds[p] == AllWorlds[i].UniqBuild[j].Id)
					{
						if (AllWorlds[i].UniqBuild[j].Type == EBuilding::PortalBuild)
						{
							if (PortalResource < 100)
							{
								PortalResource++;
								for (int t = 0; t < SavedBuilds.Num(); t++)
								{
									if (SavedBuilds.IsValidIndex(t))
									{
										if (SavedBuilds[t]->Id == AllWorlds[i].UniqBuild[j].Id)
										{
											SavedBuilds[t]->OnResourceUpdate(PortalResource);
										}
									}
								}
							}
						}
						else
						{
							if (AllWorlds[i].UniqBuild[j].SavedBuild.Resource < 100)
							{
								AllWorlds[i].UniqBuild[j].SavedBuild.Resource++;
								for (int t = 0; t < SavedBuilds.Num(); t++)
								{
									if (SavedBuilds.IsValidIndex(t))
									{
										if (SavedBuilds[t]->Id == AllWorlds[i].UniqBuild[j].Id)
										{
											SavedBuilds[t]->OnResourceUpdate(AllWorlds[i].UniqBuild[j].SavedBuild.Resource);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		for (int i = 0; i < AllWorlds.Num(); i++)
		{
			for (int j = 0; j < AllWorlds[i].UniqBuild.Num(); j++)
			{
				if (AllWorlds[i].UniqBuild[j].Type == EBuilding::PortalBuild)
				{
					AllWorlds[i].UniqBuild[j].SavedBuild.Resource = PortalResource;
				}
			}
		}
		BuildSubsystem->UpdateAllWorld(AllWorlds);
	}
	DebugFunc();
}

void UBuildGameInstance::CheckTargetBuild()
{
	UBuildingSubsystem* BuildSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (BuildSubsystem)
	{
		TArray<SUniqBuild> AllBuilds = BuildSubsystem->LoadBuilds(CurrentLevel);
		for (int i = 0; i < AllBuilds.Num(); i++)
		{
			if (AllBuilds[i].Type != EBuilding::SourceBuild)
			{
				if (AllBuilds[i].TargetId.IsEmpty())
				{
					FVector BuildLoc = AllBuilds[i].SavedBuild.BuildLocation;
					FString SavedId = "";
					float TempDist = 0;
					for (int j = 0; j < AllBuilds.Num(); j++)
					{
						if (AllBuilds[i].Id != AllBuilds[j].Id)
						{
							if (TempDist > FVector::Distance(BuildLoc, AllBuilds[j].SavedBuild.BuildLocation) || TempDist == 0)
							{
								TempDist = FVector::Distance(BuildLoc, AllBuilds[j].SavedBuild.BuildLocation);
								if(TempDist != 0)
									SavedId = AllBuilds[j].Id;
							}
						}
					}
					AllBuilds[i].TargetId = SavedId;
				}
			}
		}
		BuildSubsystem->UpdateBuilds(CurrentLevel, AllBuilds);
		for (int i = 0; i < SavedBuilds.Num(); i++)
		{
			for (int j = 0; j < AllBuilds.Num(); j++)
			{
				if (AllBuilds[j].Id == SavedBuilds[i]->Id)
				{
					SavedBuilds[i]->TargetId = AllBuilds[j].TargetId;
					//return;
				}
			}
		}
	}
}

FString UBuildGameInstance::GetTargetId(FString _Id)
{
	UBuildingSubsystem* BuildSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	FString TempString;
	if (BuildSubsystem)
	{
		TArray<SUniqBuild> AllBuilds = BuildSubsystem->LoadBuilds(CurrentLevel);
		for (int i = 0; i < AllBuilds.Num(); i++)
		{
			if (AllBuilds[i].Id == _Id)
			{
				TempString = AllBuilds[i].TargetId;
				return TempString;
			}
		}
	}
	return TempString;
}
