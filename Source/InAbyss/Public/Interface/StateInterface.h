// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INABYSS_API IStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	virtual void Damaged() = 0;
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	virtual void Die() = 0;
};