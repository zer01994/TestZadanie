// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildClassStruct.h"
#include "Components\ArrowComponent.h"
#include "BuildInterface.h"
#include "BuildingParent.generated.h"

UCLASS()
class TESTZADANIE_API ABuildingParent : public AActor, public IBuildInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingParent();

	void InitParams();

	void OnResourceUpdate(int _Resource);

	UPROPERTY(Category = Build, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(Category = Build, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EBuilding BuildType;

	UPROPERTY(Category = Build, EditAnywhere, BlueprintReadOnly)
	int Resource = 0;

	UPROPERTY(Category = Build, EditAnywhere, BlueprintReadOnly)
	FString Id;

	UPROPERTY(Category = Build, EditAnywhere, BlueprintReadOnly)
	FString TargetId = "Empty";

	UPROPERTY()
	bool LoadBuild = false;

	UPROPERTY(Category = Build, EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	UArrowComponent* Arrow;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
	bool UpdateStats(); virtual bool UpdateStats_Implementation() override;

protected:
	virtual void BeginPlay();

	FTimerHandle TickTimerHandle;

	void TickTimer();
};
