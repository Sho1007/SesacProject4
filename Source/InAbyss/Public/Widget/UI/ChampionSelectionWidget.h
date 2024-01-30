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

	virtual bool Initialize() override;

	UPROPERTY()
	class UInAbyssGameInstance* GI;

	UPROPERTY()
	class ALoginGameMode* GM;

	UPROPERTY()
	class ATempInGamePlayerController* PC;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Replicated)
	class UTextBlock* txt_PlayerName1;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Replicated)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Replicated)
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


public: // ±â´É 
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

	bool SelectChampion = false;

	void UseStartbtn();



	UFUNCTION()
	void OpenGameLevel();

public:
	UPROPERTY(Replicated)
	int32 SelectedChampionIndex = 0;

public:
	bool bIsServer = false;


};
