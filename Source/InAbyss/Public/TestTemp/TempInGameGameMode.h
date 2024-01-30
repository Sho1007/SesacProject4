// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/InGameGameMode.h"
#include "TempInGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API ATempInGameGameMode : public AInGameGameMode
{
	GENERATED_BODY()
	
public:
	ATempInGameGameMode();

	TArray<ACharacter*> SelectChampion;

};
