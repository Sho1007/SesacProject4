// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API USessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_RoomName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_HostName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_PlayerCount;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_SessionNumber;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_Join;

	int32 sessionNumber = 0;

public:
	// ------------- 세션조인 ----------------
	virtual void NativeConstruct() override;

	void Set(const struct FSessionInfo& sessionInfo);

	UFUNCTION()
	void JoinSession();
};
