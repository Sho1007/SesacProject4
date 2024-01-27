// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UI/ChampionSelectionWidget.h"
#include "InAbyssGameInstance.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>

void UChampionSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 게임 인스턴스
	GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());

	// 챔피언 선택
	btn_Champ1->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion);

}

void UChampionSelectionWidget::SetHostPlayerName()
{
	FText PlayerNameText = FText::FromString(GI->MySessionName);
	txt_PlayerName1->SetText(PlayerNameText);

}

void UChampionSelectionWidget::SetGuestPlayerName()
{
	FText PlayerNameText = FText::FromString(GI->MySessionName); // 상대방의 이름을 가져올 수 있독 변경 필요
	txt_PlayerName1->SetText(PlayerNameText);

}

void UChampionSelectionWidget::SetChampion()
{
	
}
