// Fill out your copyright notice in the Description page of Project Settings.


#include "InAbyssGameInstance.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include "TestTemp/TempInGamePlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <TestTemp/InAbyssPlayerState.h>

void UInAbyssGameInstance::Init()
{	
	Super::Init();

	PC = Cast<ATempInGamePlayerController>(GetWorld()->GetFirstPlayerController());

	// 온라인 세션 인터페이스 생성
	auto subsys = IOnlineSubsystem::Get();

	if (subsys) {
		SessionInterface = subsys->GetSessionInterface(); // 생성된 인터페이스를 변수에 저장

		// 콜백 - 특정 조건이나 상황시 호출되는 함수
		// 세션을 생성했을 때
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UInAbyssGameInstance::OnCreateSessionComplete);

		// 세션을 찾았을 때
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UInAbyssGameInstance::OnFindSessionComplete);

		// 세션에 입장했을 때
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UInAbyssGameInstance::OnJoinSessionCompleted);
	}

}

void UInAbyssGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 동기화할 변수 등록
	DOREPLIFETIME(UInAbyssGameInstance, ReadyPlayer);
}

void UInAbyssGameInstance::CreateGameSession(const FString roomName/*, const int32 playerCount*/)
{
	
	// 세션 설정 =======================================
	FOnlineSessionSettings SessionSettings;

	// 1. Dedicated server 사용하지 않음
	SessionSettings.bIsDedicated = false;

	// 2. 랜선매칭(로컬), 스팀매칭 사용할지 여부
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName(); // 서브시스템 이름 저장
	SessionSettings.bIsLANMatch = subsysName == "NULL"; // 다른 곳과 온라인으로 연결되지 않았을 경우 사용

	// 3. 매칭이 온라인을 통해 노출 허용
	SessionSettings.bShouldAdvertise = true;

	// 4. 온라인 상태 정보를 활용
	SessionSettings.bUsesPresence = true;

	// 5. 게임 진행 중에 join 가능 
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;

	// 6. 최대 허용 인원 수
	//SessionSettings.NumPublicConnections = playerCount; 
	SessionSettings.NumPublicConnections = 2; // 임시로 2명


	// 7. 커스텀 옵션 -> 롬네임
	SessionSettings.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("HOST_NAME"), MySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// netid
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	// 세션 설정 =======================================

	
	// *세션 생성
	SessionInterface->CreateSession(*netID, FName(MySessionName), SessionSettings);

}

void UInAbyssGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	// 생성한 세션에 입장 - 입장 경로 잘 적어야 함(레벨 경로 || "/Game/KHS/KHSMap.KHSMap'") 
	if (bWasSuccessful) {
		GetWorld()->ServerTravel(TEXT("/Game/KHS/Map/ChampionSelectionMap?listen"), true); // => 챔피언 선택 맵으로 이동 //
	}//"Game/OSH/Level/L_Laboratory?listen'"

}

void UInAbyssGameInstance::SetPlayerChampion(FName PlayerSessionName, FString ChampionName)
{
	SelectedChampionMap.Add(PlayerSessionName, ChampionName);
}

void UInAbyssGameInstance::SetPlayerName(APlayerState* PlayerState)
{
	PlayerNameMap.Add(PlayerState->GetUniqueId().ToString(), PlayerState->GetPlayerName());
}

void UInAbyssGameInstance::FindGameSessions()
{
	
	// 검색조건설정 ================================================== 
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. presence 설정이 true인 세션만 검색 가능
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 2. 다른 온라인과 연결되지 않은 것만 검색 가능
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	// 3. 최대 검색 세션 수
	SessionSearch->MaxSearchResults = 10;
	// 검색조건설정 ================================================== 



	// *검색 실행
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

	// -------------------------
	
	// 방 검색 중임을 표시
	onSearchState.Broadcast(true);

}

void UInAbyssGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	
	if (bWasSuccessful == false) {

		//PRINTLOG(TEXT("Session search failed..."));
		return;
	}

	// 세션검색결과
	auto results = SessionSearch->SearchResults;
	//PRINTLOG(TEXT("Search Result Count : %d"), results.Num());

	for (int i = 0; i < results.Num(); i++) {
		auto sr = results[i];
		if (sr.IsValid() == false) {
			continue;
		}

		FSessionInfo sessionInfo;
		sessionInfo.index = i;

		// 1. 방이름
		//FString roomName;
		sr.Session.SessionSettings.Get(FName("ROOM_NAME"), sessionInfo.roomName);

		// 2. 호스트이름
		//FString hostName;
		sr.Session.SessionSettings.Get(FName("HOST_NAME"), sessionInfo.hostName);

		// pc 소유자 이름
		FString ownerName = sr.Session.OwningUserName;

		// 3. 플레이어수 (최대가능수 - 현재입장가능수)
		int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;
		int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;

		sessionInfo.playerCount = FString::Printf(TEXT("(%d/%d)"), currentPlayerCount, maxPlayerCount);

		//PRINTLOG(TEXT("%s"), *sessionInfo.ToString());

		// 슬롯 추가
		// LoginWidget
		// 레벨블루프린트에서 인스턴스 가져와서


		// -------------------------------------------

		// 방검색 완료임을 표시
		onSearchCompleted.Broadcast(sessionInfo);

	}

	onSearchState.Broadcast(false);

}

void UInAbyssGameInstance::JoinSelectedSession(int32 roomIndex)
{

	auto sr = SessionSearch->SearchResults[roomIndex];

	SessionInterface->JoinSession(0, FName(MySessionName), sr);

}

void UInAbyssGameInstance::OnJoinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	
	// Client 가 방으로 들어가야 한다.
	// -> Client Travel
	if (result == EOnJoinSessionCompleteResult::Success) {
		
		// 서버가 만든 세션의 url이 필요
		FString url;
		SessionInterface->GetResolvedConnectString(sessionName, url);

		//PRINTLOG(TEXT("Client Travel url : %s"), *url);

		if (url.IsEmpty() == false) {

			// 방에 입장
			auto pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);

		}

	}
	else
	{
		//PRINTLOG(TEXT("Join session failed... %d"), result);
	}


}

void UInAbyssGameInstance::AddReadyPlayer()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Success3"));

	//ReadyPlayer++;

	UE_LOG(LogTemp, Warning, TEXT("ReadyPlayer : %d"), ReadyPlayer)


	GetWorld()->GetFirstPlayerController()->GetActorNameOrLabel();

	PC = Cast<ATempInGamePlayerController>(GetWorld()->GetFirstPlayerController());

	if (ReadyPlayer >= 2) {
		PC->ActivateStartButton();

		for (auto Iter : SelectedChampionMap)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInAbyssGameInstance::AddReadyPlayer : PlayerName : %s, Champion Name : %s"), *Iter.Key.ToString(), *Iter.Value);
		}
	}

}
