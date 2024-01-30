// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/InGameGameMode.h"

#include <GameFramework/PlayerState.h>
#include <EngineUtils.h>

#include "InAbyssGameInstance.h"
#include "Component/StateComponentBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	TActorIterator<APlayerStart> Iter(GetWorld());

	for (; Iter; ++Iter)
	{
		if (Iter->GetActorNameOrLabel().Compare(TEXT("BlueTeamStart")) == 0)
		{
			BlueTeamStart = *Iter;
		}
		else if (Iter->GetActorNameOrLabel().Compare(TEXT("RedTeamStart")) == 0)
		{
			RedTeamStart = *Iter;
		}
	}
}

void AInGameGameMode::SpawnChampoin(APlayerState* PlayerState)
{
	APawn* OldPawn = PlayerState->GetPlayerController()->GetPawn();
	FString SelectedChampion =  GetGameInstance<UInAbyssGameInstance>()->GetPlayerChampion(FName(PlayerState->GetPlayerName()));

	if (ChampionClassMap.Contains(SelectedChampion) == false) return;

	ACharacter* PlayerCharacter;
	
	if (PlayerState->GetPlayerController()->IsLocalController())
	{
		PlayerCharacter =  GetWorld()->SpawnActor<ACharacter>(ChampionClassMap[SelectedChampion], BlueTeamStart->GetActorLocation(), BlueTeamStart->GetActorRotation());
		if (UStateComponentBase* StateComponent = PlayerCharacter->GetComponentByClass<UStateComponentBase>())
		{
			StateComponent->SetFactionType(EFactionType::BLUE);
			StateComponent->SetObjectType(EObjectType::CHAMPION);
		}
	}
	else
	{
		PlayerCharacter =  GetWorld()->SpawnActor<ACharacter>(ChampionClassMap[SelectedChampion], RedTeamStart->GetActorLocation(), RedTeamStart->GetActorRotation());
		if (UStateComponentBase* StateComponent = PlayerCharacter->GetComponentByClass<UStateComponentBase>())
		{
			StateComponent->SetFactionType(EFactionType::RED);
			StateComponent->SetObjectType(EObjectType::CHAMPION);
		}
	}
	PlayerState->GetPlayerController()->Possess(PlayerCharacter);
	OldPawn->Destroy();
}
