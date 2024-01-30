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

	// 게임 인스턴스
	GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());
	
	// 검색 중일 때 호출
	GI->onSearchCompleted.AddDynamic(this, &ULoginWidget::AddSlotwidget);

	// 검색 완료시 호출
	GI->onSearchState.AddDynamic(this, &ULoginWidget::OnChangeButtonEnable);

	// Main --------------------------------------------------------
	// 방만들기 공간으로 이동
	btn_CreateGameRoom_Ready->OnClicked.AddDynamic(this, &ULoginWidget::SwitchCreateRoomPanel);

	// 방 찾기
	btn_FindGame->OnClicked.AddDynamic(this, &ULoginWidget::SwitchFindPanel);

	// Create Room --------------------------------------------------------

	// 방 만들기
	btn_CreateGameRoom->OnClicked.AddDynamic(this, &ULoginWidget::CreateGameRoom);

	/*
	Slider_PlayerNumber->OnValueChanged.AddDynamic(this, &ULoginWidget::OnValueChanged);
	*/

	// Find Room --------------------------------------------------------

	btn_ReFindGame->OnClicked.AddDynamic(this, &ULoginWidget::FindGameRoom);





	// 뒤로 가기
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

	// 방목록 검색
	FindGameRoom();
}

// ========================================================================

void ULoginWidget::CreateGameRoom()
{
	if (GI && edit_RoomName->GetText().IsEmpty() == false) {
		// 방제목
		FString roomName = edit_RoomName->GetText().ToString();
		
		/*
		// 방 인원수
		int32 playerCount = Slider_PlayerNumber->GetValue();
		*/

		// 비밀번호
		//FString roomName = edit_Password->GetText().ToString();

		// 세션 생성
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
	// 위젯 생성
	auto slot = CreateWidget<USessionSlotWidget>(this, sessionSlotWidget);
	// 데이터 할당
	slot->Set(sessionInfo);
	// 스크롤 박스에 추가
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
