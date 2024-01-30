// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameGameMode.generated.h"

/**
 * 
 */
class APlayerStart;
UCLASS()
class INABYSS_API AInGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void SpawnChampoin(APlayerState* PlayerState);

private:
	UPROPERTY(EditDefaultsOnly,  Meta = (AllowPrivateAccess))
	TMap<FString, TSubclassOf<ACharacter>> ChampionClassMap;

	UPROPERTY(VisibleInstanceOnly,  Meta = (AllowPrivateAccess))
	APlayerStart* BlueTeamStart;
	UPROPERTY(VisibleInstanceOnly,  Meta = (AllowPrivateAccess))
	APlayerStart* RedTeamStart;
};