// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/InGamePlayerController.h"
#include "TempInGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API ATempInGamePlayerController : public AInGamePlayerController
{
	GENERATED_BODY()
	
public:
	ATempInGamePlayerController();

	virtual void BeginPlay() override;

	UPROPERTY()
	class UInAbyssGameInstance* GI;

	

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY()
	class UChampionSelectionWidget* ChampionSelectionWidget;

public:
	 
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerChampion(const FString& NewPlayerChampion);


	UFUNCTION()
	void CallPlayerNum();

	UFUNCTION(Server, Reliable)
	void ServerRPCCallPlayerNum();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCallPlayerNum(int Num);

	UFUNCTION(Client, Reliable)
	void ClientRPCCallPlayerNum(int Num);

	void ActivateStartButton();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChampionSelectionWidget> ChampionSelectionWidgetClass;

public:
	//UPROPERTY(ReplicatedUsing=OnRep_ReadyNum)
	UPROPERTY(Replicated)
	int32 ReadyNum = 0;
	UFUNCTION()
	void OnRep_ReadyNum();
};
