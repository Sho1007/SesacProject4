// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "InAbyssGameInstance.generated.h"

// ������(����) ����� ����ü
USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()
	// �� �̸�
	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	// �� ���� �̸�
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	// �濡 �ִ� �÷��̾� ��
	UPROPERTY(BlueprintReadOnly)
	FString playerCount;
	// Session�� index
	UPROPERTY(BlueprintReadOnly)
	int32 index;

	inline FString ToString()
	{
		return FString::Printf(TEXT("[%d] %s : %s - %s"), index, *roomName, *hostName, *playerCount);
	}

};

// ���� �˻��� ������ ȣ��� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const FSessionInfo&, sessionInfo);

// ���� �˻� ���� ���� �� ȣ��� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSignature, bool, bIsSearching);

/**
 * 
 */
UCLASS()
class INABYSS_API UInAbyssGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// �ν��Ͻ� �ʱ�ȭ �ϴ� ���
	virtual void Init() override;

	// ���� ����
	IOnlineSessionPtr SessionInterface;

	// ����(ȣ��Ʈ)�̸�
	FString MySessionName = "InAbyss";

	class ATempInGamePlayerController* PC;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public: // ���� ���� ���
	// ���� �� ����� ���
	void CreateGameSession(const FString roomName/*, const int32 playerCount*/);

	// �� ���� �Ϸ�� �ݹ�
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void SetPlayerChampion(FName PlayerSessionName, FString ChampionName);
	void SetPlayerName(APlayerState* PlayerState);
	FString GetPlayerChampion(FName PlayerName);
public:
	// �˻� ���ǵ�
	TSharedPtr<FOnlineSessionSearch > SessionSearch;

	// ���� �˻� ��� 
	void FindGameSessions();

	// ���ǰ˻� �Ϸ�� �ݹ�
	void OnFindSessionComplete(bool bWasSuccessful);

	// ��ã�� �Ϸ� �ݹ� ��������Ʈ
	FSearchSignature onSearchCompleted;

	// �� ã�� �߿� �ݹ� ��������Ʈ
	FSearchStateSignature onSearchState;

public:	// ���� ���� ���
	// ����(��) ���� ���
	void JoinSelectedSession(int32 roomIndex);

	// ���� ����� �ݹ�
	void OnJoinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type result);

public:
	TMap<FName, FString> SelectedChampionMap;
	TMap<FString, FString> PlayerNameMap;

	UPROPERTY(Replicated)
	int32 ReadyPlayer = 0;

	UFUNCTION()
	void AddReadyPlayer();
};
