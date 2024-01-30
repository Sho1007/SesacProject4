// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InAbyssPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API AInAbyssPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerName(const FString& NewPlayerName);

	virtual void BeginPlay() override;
};