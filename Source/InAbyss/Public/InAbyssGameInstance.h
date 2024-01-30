// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "InAbyssGameInstance.generated.h"

// 방정보(세션) 기록할 구조체
USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()
	// 방 이름
	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	// 방 주인 이름
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	// 방에 있는 플레이어 수
	UPROPERTY(BlueprintReadOnly)
	FString playerCount;
	// Session의 index
	UPROPERTY(BlueprintReadOnly)
	int32 index;

	inline FString ToString()
	{
		return FString::Printf(TEXT("[%d] %s : %s - %s"), index, *roomName, *hostName, *playerCount);
	}

};

// 세션 검색이 끝나면 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const FSessionInfo&, sessionInfo);

// 세션 검색 상태 중일 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSignature, bool, bIsSearching);

/**
 * 
 */
UCLASS()
class INABYSS_API UInAbyssGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 인스턴스 초기화 하는 기능
	virtual void Init() override;

	// 세션 설정
	IOnlineSessionPtr SessionInterface;

	// 세션(호스트)이름
	FString MySessionName = "InAbyss";

	class ATempInGamePlayerController* PC;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public: // 세션 생성 기능
	// 게임 방 만드는 기능
	void CreateGameSession(const FString roomName/*, const int32 playerCount*/);

	// 방 생성 완료시 콜백
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

public:
	// 검색 조건들
	TSharedPtr<FOnlineSessionSearch > SessionSearch;

	// 세션 검색 기능 
	void FindGameSessions();

	// 세션검색 완료시 콜백
	void OnFindSessionComplete(bool bWasSuccessful);

	// 방찾기 완료 콜백 델리게이트
	FSearchSignature onSearchCompleted;

	// 방 찾는 중에 콜백 델리게이트
	FSearchStateSignature onSearchState;

public:	// 세션 입장 기능
	// 세션(방) 입장 기능
	void JoinSelectedSession(int32 roomIndex);

	// 세션 입장시 콜백
	void OnJoinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type result);

public:
	UPROPERTY(Replicated)
	int32 ReadyPlayer = 0;

	UFUNCTION()
	void AddReadyPlayer();
};
