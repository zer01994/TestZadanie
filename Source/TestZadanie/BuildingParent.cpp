// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingParent.h"
#include "BuildGameInstance.h"

// Sets default values
ABuildingParent::ABuildingParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(StaticMesh);
}

void ABuildingParent::InitParams()
{
	UBuildGameInstance* GI = Cast<UBuildGameInstance>(GetWorld()->GetGameInstance());
	if (!GI)
	{
		return;
	}
	if (!LoadBuild)
	{
		SUniqBuild UniqBuild;
		UniqBuild.SavedBuild.BuildLocation = GetActorLocation();
		UniqBuild.SavedBuild.BuildRotation = GetActorRotation();
		UniqBuild.SavedBuild.Resource = Resource;
		UniqBuild.Type = BuildType;
		Id = FString::FromInt(FRandomStream(FName(*FString::FromInt(GI->CurrentLevel) + GetActorLocation().GetSafeNormal().ToString() + GetActorRotation().GetNormalized().ToString() + FDateTime::Now().ToString())).GetCurrentSeed());
		if (BuildType == EBuilding::PortalBuild)
			Id = FString("Portal_") + Id;
		UniqBuild.Id = Id;
		GI->AddBuild(GI->CurrentLevel, UniqBuild);
		GI->CheckTargetBuild();
		TargetId = GI->GetTargetId(Id);
	}
}

void ABuildingParent::OnResourceUpdate(int _Resource)
{
	Resource = _Resource;
}

bool ABuildingParent::UpdateStats_Implementation()
{
	return false;
}

void ABuildingParent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ABuildingParent::TickTimer, 0.1f, true);
}

void ABuildingParent::TickTimer()
{
	UpdateStats();
}

