// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UI/ChampionSelectionWidget.h"
#include "InAbyssGameInstance.h"
#include <GameFramework/PlayerState.h>
#include <GameFramework/PlayerController.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include "GameMode/LoginGameMode.h"
#include <Net/UnrealNetwork.h>
#include "TestTemp/TempInGamePlayerController.h"
#include <InAbyss/InAbyss.h>


void UChampionSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	// è�Ǿ� ����
	btn_Champ1->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion1);
	btn_Champ2->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion2);
	btn_Champ3->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion3);

	// ���� ���� - ���� ���� ����
	btn_Selection->OnClicked.AddDynamic(this, &UChampionSelectionWidget::OpenGameLevel);

	

	if (GM && GM->HasAuthority()) {
		bIsServer = true;
		
		btn_Selection->SetIsEnabled(false);

	}

	if (bIsServer == true) { // �������
		SetHostPlayerName();
	}
	else if (bIsServer == false) { // Ŭ������Ʈ���
		SetGuestPlayerName();
		btn_Selection->SetVisibility(ESlateVisibility::Hidden);
	}

}

bool UChampionSelectionWidget::Initialize()
{
	if (Super::Initialize() == false) return false;

	// ���� �ν��Ͻ�
	GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());
	GM = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());
	
	PC = GetWorld()->GetFirstPlayerController<ATempInGamePlayerController>();
	

	return true;
}

void UChampionSelectionWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // ���� ����ȭ ó��
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ����ȭ�� ���� ���
	DOREPLIFETIME(UChampionSelectionWidget, txt_PlayerName1);
	DOREPLIFETIME(UChampionSelectionWidget, txt_PlayerName2);
	DOREPLIFETIME(UChampionSelectionWidget, SelectedChampionIndex);



}

void UChampionSelectionWidget::SetHostPlayerName()
{
	FText PlayerNameText = FText::FromString(GI->MySessionName);
	txt_PlayerName1->SetText(PlayerNameText);

}


void UChampionSelectionWidget::SetGuestPlayerName()
{

	FText PlayerNameText = FText::FromString(GI->MySessionName);
	txt_PlayerName2->SetText(PlayerNameText);

}


// ---------------------------------------------------------------

void UChampionSelectionWidget::SetChampion1()
{
	SelectedChampionIndex = 1;
	SetChampion(SelectedChampionIndex);
}

void UChampionSelectionWidget::SetChampion2()
{
	SelectedChampionIndex = 2;
	SetChampion(SelectedChampionIndex);
}

void UChampionSelectionWidget::SetChampion3()
{
	SelectedChampionIndex = 3;
	SetChampion(SelectedChampionIndex);
}

void UChampionSelectionWidget::SetChampion(int32 ChampionIndex)
{
	
	FString ChampionName;

	// �ε����� ���� ���õ� è�Ǿ��� ����
	switch (ChampionIndex)
	{
	case 1:
		ChampionName = TEXT("Garen");
		break;
	case 2:
		ChampionName = TEXT("Lux");
		break;
	case 3:
		ChampionName = TEXT("Ezreal");
		break;
	default:
		ChampionName = TEXT("Champion Name");
		break;
	}


	// ���õ� è�Ǿ��� �̸��� �ؽ�Ʈ ��Ͽ� ǥ��
	FText ChampionText = FText::FromString(ChampionName);
	txt_Champion->SetText(ChampionText);

	
	//Cast<ATempInGamePlayerController>(GetOwningPlayer())->ServerRPCCallPlayerNum();


	Cast<ATempInGamePlayerController>(GetOwningPlayer())->CallPlayerNum();
	Cast<ATempInGamePlayerController>(GetOwningPlayer())->ServerRPC_SetPlayerChampion(ChampionName);


	// PRINTLOG(TEXT("PlayerName : %s"), *GetOwningPlayer()->GetPlayerState()->GetPlayerName());
	UE_LOG(LogTemp, Warning, TEXT("UChampionSelectionWidget::SetChampion) PlayerName : %s"), *GetOwningPlayer()->GetPlayerState<APlayerState>()->GetPlayerName());

	//




	//UE_LOG(LogTemp, Warning, TEXT("====================End===================="));

	// 

}

void UChampionSelectionWidget::UseStartbtn()
{
	btn_Selection->SetIsEnabled(true);
}


void UChampionSelectionWidget::OpenGameLevel()
{
	if (SelectedChampionIndex == 0) {
		return;
	}

	// �̵��� ���� ����
	FString NextLevelName = TEXT("L_Laboratory"); // KHSMap // L_Laboratory // NewWorld_JSH

	// ������ �̵���Ŵ
	GetWorld()->ServerTravel(*NextLevelName);
}