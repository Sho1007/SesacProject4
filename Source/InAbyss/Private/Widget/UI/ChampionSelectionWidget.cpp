// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UI/ChampionSelectionWidget.h"
#include "InAbyssGameInstance.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include "GameMode/LoginGameMode.h"
#include <Net/UnrealNetwork.h>


void UChampionSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �ν��Ͻ�
	GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());

	// è�Ǿ� ����
	btn_Champ1->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion1);
	btn_Champ2->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion2);
	btn_Champ3->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion3);

	// ���� ����
	btn_Selection->OnClicked.AddDynamic(this, &UChampionSelectionWidget::OpenGameLevel);



	gm = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());

	if (gm && gm->HasAuthority()) {
		bIsServer = true;
	}

	if (bIsServer == true) { // �������
		SetHostPlayerName();
	}
	else if (bIsServer == false) { // Ŭ������Ʈ���
		SetGuestPlayerName();
	}

}

void UChampionSelectionWidget::SetHostPlayerName()
{
	FText PlayerNameText = FText::FromString(GI->MySessionName);
	txt_PlayerName1->SetText(PlayerNameText);

}

void UChampionSelectionWidget::ServerRPCSetHostPlayerName_Implementation()
{

}

void UChampionSelectionWidget::MultiRPCSetHostPlayerName_Implementation(const FText& PlayerName)
{

}

void UChampionSelectionWidget::SetGuestPlayerName()
{

	ServerRPCSetGuestPlayerName();

}

void UChampionSelectionWidget::ServerRPCSetGuestPlayerName_Implementation()
{
	FText PlayerNameText = FText::FromString(GI->MySessionName);

	MultiRPCSetGuestPlayerName(PlayerNameText);

}

void UChampionSelectionWidget::MultiRPCSetGuestPlayerName_Implementation(const FText& PlayerName)
{

	txt_PlayerName2->SetText(PlayerName);

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
		ChampionName = TEXT("NoneChampion");
		break;
	}


	// ���õ� è�Ǿ��� �̸��� �ؽ�Ʈ ��Ͽ� ǥ��
	FText ChampionText = FText::FromString(ChampionName);
	txt_Champion->SetText(ChampionText);


	// ������ è�Ǿ��� �����ϰ�, ���� �������� ������ �� �ֵ��� �����ؾ� ��


}


void UChampionSelectionWidget::OpenGameLevel()
{
	if (SelectedChampionIndex == 0) {
		return;
	}

	ServerRPCOpenGameLevel();
}


void UChampionSelectionWidget::ServerRPCOpenGameLevel_Implementation()
{
	//FString Options = FString::Printf(TEXT("SelectedChampion=%d"), SelectedChampionIndex); // ������ ĳ���� �ε����� ���ڿ��� ��ȯ�Ͽ� ����
	
	// �̵��� ���� ����
	FString NextLevelName = TEXT("L_Laboratory"); // KHSMap

	// ������ �̵���Ŵ
	GetWorld()->ServerTravel(*NextLevelName);


}

void UChampionSelectionWidget::MultiRPCOpenGameLevel_Implementation(const FText& PlayerName)
{

}

void UChampionSelectionWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // ���� ����ȭ ó��
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ����ȭ�� ���� ���
	DOREPLIFETIME(UChampionSelectionWidget, txt_PlayerName1);
	DOREPLIFETIME(UChampionSelectionWidget, txt_PlayerName2);
	DOREPLIFETIME(UChampionSelectionWidget, SelectedChampionIndex);


}

void UChampionSelectionWidget::OnRep_UpdatePlayerName1()
{
	


}

void UChampionSelectionWidget::OnRep_UpdatePlayerName2()
{


}