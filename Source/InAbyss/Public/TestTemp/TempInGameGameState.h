// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/InGameGameState.h"
#include "TempInGameGameState.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API ATempInGameGameState : public AInGameGameState
{
	GENERATED_BODY()
	
public:		
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class APlayerController* PC;

	UPROPERTY()
    class UChampionSelectionWidget* ChampionSelectionWidget; 


public:
    UPROPERTY(Replicated)
	int32 ClientsReady = 0;




};
