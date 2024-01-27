// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UI/ChampionSelectionWidget.h"
#include "InAbyssGameInstance.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>

void UChampionSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �ν��Ͻ�
	GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());

	// è�Ǿ� ����
	btn_Champ1->OnClicked.AddDynamic(this, &UChampionSelectionWidget::SetChampion);

}

void UChampionSelectionWidget::SetHostPlayerName()
{
	FText PlayerNameText = FText::FromString(GI->MySessionName);
	txt_PlayerName1->SetText(PlayerNameText);

}

void UChampionSelectionWidget::SetGuestPlayerName()
{
	FText PlayerNameText = FText::FromString(GI->MySessionName); // ������ �̸��� ������ �� �ֵ� ���� �ʿ�
	txt_PlayerName1->SetText(PlayerNameText);

}

void UChampionSelectionWidget::SetChampion()
{
	
}
