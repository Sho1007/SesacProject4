// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};