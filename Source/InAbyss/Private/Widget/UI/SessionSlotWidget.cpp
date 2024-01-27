// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UI/SessionSlotWidget.h"
#include "InAbyssGameInstance.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>

void USessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Join->OnClicked.AddDynamic(this, &USessionSlotWidget::JoinSession);
}

void USessionSlotWidget::Set(const FSessionInfo& sessionInfo)
{
	// 技记(规) 锅龋
	txt_SessionNumber->SetText(FText::FromString(FString::Printf(TEXT("[%d]"), sessionInfo.index)));

	// 规捞抚
	txt_RoomName->SetText(FText::FromString(sessionInfo.roomName));

	txt_HostName->SetText(FText::FromString(sessionInfo.hostName));

	txt_PlayerCount->SetText(FText::FromString(sessionInfo.playerCount));

	sessionNumber = sessionInfo.index;


}

void USessionSlotWidget::JoinSession()
{
	auto GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());

	if (GI) {
		GI->JoinSelectedSession(sessionNumber);
	}
}
