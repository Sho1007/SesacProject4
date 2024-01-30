// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTemp/InAbyssPlayerState.h"
#include <InAbyss/InAbyss.h>
#include <InAbyssGameInstance.h>

#include "GameMode/InGameGameMode.h"

void AInAbyssPlayerState::ServerRPC_SetPlayerName_Implementation(const FString& NewPlayerName)
{
	SetPlayerName(NewPlayerName);

	AInGameGameMode* CurrentGameMode = Cast<AInGameGameMode>(GetWorld()->GetAuthGameMode());
	if (CurrentGameMode && CurrentGameMode->IsValidLowLevel())
	{
		CurrentGameMode->SpawnChampoin(this);
	}
}

void AInAbyssPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// UE_LOG(LogTemp, Warning, TEXT("AInAbyssPlayerState::BeginPlay) PlayerStateName : %s"), *GetActorNameOrLabel());

	APlayerController* PC = GetPlayerController();
	if (PC && PC->IsLocalController())
	{
		ServerRPC_SetPlayerName(GetGameInstance<UInAbyssGameInstance>()->MySessionName);
		UE_LOG(LogTemp, Warning, TEXT("AInAbyssPlayerState::BeginPlay) MySessionName : %s"), *GetGameInstance<UInAbyssGameInstance>()->MySessionName);
	}
}