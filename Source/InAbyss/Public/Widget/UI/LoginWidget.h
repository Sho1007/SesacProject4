// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UInAbyssGameInstance* GI;

public: // Main
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_ID;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_CreateGameRoom_Ready;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_FindGame;

public:
	UFUNCTION()
	void SwitchCreateRoomPanel();
	
	UFUNCTION()
	void SwitchFindPanel();

public: // Create Room
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_RoomName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_Password;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_PlayerCount;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_CreateGameRoom;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class USlider* Slider_PlayerNumber;

public:
	// �� ���� ��� - è�Ǿ� ���� ������ �̵�
	UFUNCTION()
	void CreateGameRoom();
	
	// PlayerCount ���� ��� - Slider
	UFUNCTION()
	void OnValueChanged(float value);

public: // Find Room
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_Finding_Msg;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_ReFindGame;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UScrollBox* Scroll_RoomList;

	
	// ���ǽ��� ����
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USessionSlotWidget> sessionSlotWidget;


public:
	// �� ã�� ���
	UFUNCTION()
	void FindGameRoom();

	UFUNCTION()
	void AddSlotwidget(const struct FSessionInfo& sessionInfo);

	// ��ã�� ���� �̺�Ʈ �ݹ�
	UFUNCTION()
	void OnChangeButtonEnable(bool bIsSeraching);

public: // ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;


	

	UFUNCTION()
	void BackToMain();



};
