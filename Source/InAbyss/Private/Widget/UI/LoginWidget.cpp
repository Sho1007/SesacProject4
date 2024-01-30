// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UI/LoginWidget.h"
#include "InAbyssGameInstance.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/EditableText.h>
#include <Components/Slider.h>
#include <Components/WidgetSwitcher.h>
#include <Components/ScrollBox.h>
#include "Widget/UI/SessionSlotWidget.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �ν��Ͻ�
	GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());
	
	// �˻� ���� �� ȣ��
	GI->onSearchCompleted.AddDynamic(this, &ULoginWidget::AddSlotwidget);

	// �˻� �Ϸ�� ȣ��
	GI->onSearchState.AddDynamic(this, &ULoginWidget::OnChangeButtonEnable);

	// Main --------------------------------------------------------
	// �游��� �������� �̵�
	btn_CreateGameRoom_Ready->OnClicked.AddDynamic(this, &ULoginWidget::SwitchCreateRoomPanel);

	// �� ã��
	btn_FindGame->OnClicked.AddDynamic(this, &ULoginWidget::SwitchFindPanel);

	// Create Room --------------------------------------------------------

	// �� �����
	btn_CreateGameRoom->OnClicked.AddDynamic(this, &ULoginWidget::CreateGameRoom);

	/*
	Slider_PlayerNumber->OnValueChanged.AddDynamic(this, &ULoginWidget::OnValueChanged);
	*/

	// Find Room --------------------------------------------------------

	btn_ReFindGame->OnClicked.AddDynamic(this, &ULoginWidget::FindGameRoom);





	// �ڷ� ����
	btn_BackMain->OnClicked.AddDynamic(this, &ULoginWidget::BackToMain);
	btn_BackMain_1->OnClicked.AddDynamic(this, &ULoginWidget::BackToMain);

}

// ========================================================================

void ULoginWidget::SwitchCreateRoomPanel()
{
	FString userName = edit_ID->GetText().ToString();

	if (userName.IsEmpty() == false) {

		GI->MySessionName = userName;

	}

	WidgetSwitcher->SetActiveWidgetIndex(1);

}

void ULoginWidget::SwitchFindPanel()
{
	FString userName = edit_ID->GetText().ToString();

	if (userName.IsEmpty() == false) {

		GI->MySessionName = userName;

	}

	WidgetSwitcher->SetActiveWidgetIndex(2);

	// ���� �˻�
	FindGameRoom();
}

// ========================================================================

void ULoginWidget::CreateGameRoom()
{
	if (GI && edit_RoomName->GetText().IsEmpty() == false) {
		// ������
		FString roomName = edit_RoomName->GetText().ToString();
		
		/*
		// �� �ο���
		int32 playerCount = Slider_PlayerNumber->GetValue();
		*/

		// ��й�ȣ
		//FString roomName = edit_Password->GetText().ToString();

		// ���� ����
		GI->CreateGameSession(roomName/*, playerCount*/);
	}

}

/*
void ULoginWidget::OnValueChanged(float value)
{
	// Player
	txt_PlayerCount->SetText(FText::AsNumber(value));

}
*/

void ULoginWidget::BackToMain()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);

}

// ========================================================================


void ULoginWidget::FindGameRoom()
{
	Scroll_RoomList->ClearChildren();

	if (GI) {
		GI->FindGameSessions();
	}

}

void ULoginWidget::AddSlotwidget(const FSessionInfo& sessionInfo)
{
	// ���� ����
	auto slot = CreateWidget<USessionSlotWidget>(this, sessionSlotWidget);
	// ������ �Ҵ�
	slot->Set(sessionInfo);
	// ��ũ�� �ڽ��� �߰�
	Scroll_RoomList->AddChild(slot);
	
}

void ULoginWidget::OnChangeButtonEnable(bool bIsSeraching)
{
	btn_ReFindGame->SetIsEnabled(!bIsSeraching);

	if (bIsSeraching) {
		txt_Finding_Msg->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		txt_Finding_Msg->SetVisibility(ESlateVisibility::Hidden);
	}
}
