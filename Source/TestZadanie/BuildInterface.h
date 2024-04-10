// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UBuildInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTZADANIE_API IBuildInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trigger")
	bool UpdateStats();
};
