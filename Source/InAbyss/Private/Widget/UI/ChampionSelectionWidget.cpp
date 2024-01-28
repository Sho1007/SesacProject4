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

	// 게임 인스턴스
	GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());

	// 챔피언 선택
	btn_Champ1->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion1);
	btn_Champ2->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion2);
	btn_Champ3->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion3);

	// 게임 시작
	btn_Selection->OnClicked.AddDynamic(this, &UChampionSelectionWidget::OpenGameLevel);



	gm = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());

	if (gm && gm->HasAuthority()) {
		bIsServer = true;
	}

	if (bIsServer == true) { // 서버라면
		SetHostPlayerName();
	}
	else if (bIsServer == false) { // 클라이턴트라면
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

	// 인덱스에 따라 선택된 챔피언을 결정
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


	// 선택된 챔피언의 이름을 텍스트 블록에 표시
	FText ChampionText = FText::FromString(ChampionName);
	txt_Champion->SetText(ChampionText);


	// 선택한 챔피언을 저장하고, 다음 레벨에서 스폰할 수 있도록 전달해야 함


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
	//FString Options = FString::Printf(TEXT("SelectedChampion=%d"), SelectedChampionIndex); // 선택한 캐릭터 인덱스를 문자열로 변환하여 전달
	
	// 이동할 레벨 지정
	FString NextLevelName = TEXT("L_Laboratory"); // KHSMap

	// 레벨을 이동시킴
	GetWorld()->ServerTravel(*NextLevelName);


}

void UChampionSelectionWidget::MultiRPCOpenGameLevel_Implementation(const FText& PlayerName)
{

}

void UChampionSelectionWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // 변수 동기화 처리
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 동기화할 변수 등록
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