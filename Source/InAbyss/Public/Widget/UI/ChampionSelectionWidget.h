// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChampionSelectionWidget.generated.h"

/**
 *
 */
UCLASS()
class INABYSS_API UChampionSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UInAbyssGameInstance* GI;

	UPROPERTY()
	class ALoginGameMode* gm;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), ReplicatedUsing = OnRep_UpdatePlayerName1)
	class UTextBlock* txt_PlayerName1;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Replicated)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), ReplicatedUsing = OnRep_UpdatePlayerName2)
	class UTextBlock* txt_PlayerName2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_Champion;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_Champ1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_Champ2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_Champ3;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_Selection;


public: // 기능 
	UFUNCTION()
	void SetHostPlayerName();

	UFUNCTION()
	void SetGuestPlayerName();


	UFUNCTION()
	void SetChampion1();

	UFUNCTION()
	void SetChampion2();

	UFUNCTION()
	void SetChampion3();


	UFUNCTION()
	void SetChampion(int32 ChampionIndex);

	/*
	UFUNCTION(Server, Reliable)
	void ServerRPCSetChampion(int32 ChampionIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCSetChampion(int32 ChampionIndex);
	*/




	UFUNCTION()
	void OpenGameLevel();

public:
	UPROPERTY(Replicated)
	int32 SelectedChampionIndex = 0;

public:
	bool bIsServer = false;


public:
	UFUNCTION(Server, Reliable)
	void ServerRPCSetHostPlayerName();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCSetHostPlayerName(const FText& PlayerName);

	UFUNCTION(Server, Reliable)
	void ServerRPCSetGuestPlayerName();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCSetGuestPlayerName(const FText& PlayerName);





	UFUNCTION(Server, Reliable)
	void ServerRPCOpenGameLevel();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCOpenGameLevel(const FText& PlayerName);

public: // OnRep
	UFUNCTION()
	void OnRep_UpdatePlayerName1(); // 플레이어 1 텍스트 업데이트

	UFUNCTION()
	void OnRep_UpdatePlayerName2(); // 플레이어 2 텍스트 업데이트
};
