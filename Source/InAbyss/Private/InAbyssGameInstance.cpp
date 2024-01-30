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

	// �¶��� ���� �������̽� ����
	auto subsys = IOnlineSubsystem::Get();

	if (subsys) {
		SessionInterface = subsys->GetSessionInterface(); // ������ �������̽��� ������ ����

		// �ݹ� - Ư�� �����̳� ��Ȳ�� ȣ��Ǵ� �Լ�
		// ������ �������� ��
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UInAbyssGameInstance::OnCreateSessionComplete);

		// ������ ã���� ��
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UInAbyssGameInstance::OnFindSessionComplete);

		// ���ǿ� �������� ��
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UInAbyssGameInstance::OnJoinSessionCompleted);
	}

}

void UInAbyssGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ����ȭ�� ���� ���
	DOREPLIFETIME(UInAbyssGameInstance, ReadyPlayer);
}

void UInAbyssGameInstance::CreateGameSession(const FString roomName/*, const int32 playerCount*/)
{
	
	// ���� ���� =======================================
	FOnlineSessionSettings SessionSettings;

	// 1. Dedicated server ������� ����
	SessionSettings.bIsDedicated = false;

	// 2. ������Ī(����), ������Ī ������� ����
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName(); // ����ý��� �̸� ����
	SessionSettings.bIsLANMatch = subsysName == "NULL"; // �ٸ� ���� �¶������� ������� �ʾ��� ��� ���

	// 3. ��Ī�� �¶����� ���� ���� ���
	SessionSettings.bShouldAdvertise = true;

	// 4. �¶��� ���� ������ Ȱ��
	SessionSettings.bUsesPresence = true;

	// 5. ���� ���� �߿� join ���� 
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;

	// 6. �ִ� ��� �ο� ��
	//SessionSettings.NumPublicConnections = playerCount; 
	SessionSettings.NumPublicConnections = 2; // �ӽ÷� 2��


	// 7. Ŀ���� �ɼ� -> �ҳ���
	SessionSettings.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("HOST_NAME"), MySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// netid
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	// ���� ���� =======================================

	
	// *���� ����
	SessionInterface->CreateSession(*netID, FName(MySessionName), SessionSettings);

}

void UInAbyssGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	// ������ ���ǿ� ���� - ���� ��� �� ����� ��(���� ��� || "/Game/KHS/KHSMap.KHSMap'") 
	if (bWasSuccessful) {
		GetWorld()->ServerTravel(TEXT("/Game/KHS/Map/ChampionSelectionMap?listen"), true); // => è�Ǿ� ���� ������ �̵� //
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
	
	// �˻����Ǽ��� ================================================== 
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. presence ������ true�� ���Ǹ� �˻� ����
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 2. �ٸ� �¶��ΰ� ������� ���� �͸� �˻� ����
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	// 3. �ִ� �˻� ���� ��
	SessionSearch->MaxSearchResults = 10;
	// �˻����Ǽ��� ================================================== 



	// *�˻� ����
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

	// -------------------------
	
	// �� �˻� ������ ǥ��
	onSearchState.Broadcast(true);

}

void UInAbyssGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	
	if (bWasSuccessful == false) {

		//PRINTLOG(TEXT("Session search failed..."));
		return;
	}

	// ���ǰ˻����
	auto results = SessionSearch->SearchResults;
	//PRINTLOG(TEXT("Search Result Count : %d"), results.Num());

	for (int i = 0; i < results.Num(); i++) {
		auto sr = results[i];
		if (sr.IsValid() == false) {
			continue;
		}

		FSessionInfo sessionInfo;
		sessionInfo.index = i;

		// 1. ���̸�
		//FString roomName;
		sr.Session.SessionSettings.Get(FName("ROOM_NAME"), sessionInfo.roomName);

		// 2. ȣ��Ʈ�̸�
		//FString hostName;
		sr.Session.SessionSettings.Get(FName("HOST_NAME"), sessionInfo.hostName);

		// pc ������ �̸�
		FString ownerName = sr.Session.OwningUserName;

		// 3. �÷��̾�� (�ִ밡�ɼ� - �������尡�ɼ�)
		int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;
		int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;

		sessionInfo.playerCount = FString::Printf(TEXT("(%d/%d)"), currentPlayerCount, maxPlayerCount);

		//PRINTLOG(TEXT("%s"), *sessionInfo.ToString());

		// ���� �߰�
		// LoginWidget
		// �����������Ʈ���� �ν��Ͻ� �����ͼ�


		// -------------------------------------------

		// ��˻� �Ϸ����� ǥ��
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
	
	// Client �� ������ ���� �Ѵ�.
	// -> Client Travel
	if (result == EOnJoinSessionCompleteResult::Success) {
		
		// ������ ���� ������ url�� �ʿ�
		FString url;
		SessionInterface->GetResolvedConnectString(sessionName, url);

		//PRINTLOG(TEXT("Client Travel url : %s"), *url);

		if (url.IsEmpty() == false) {

			// �濡 ����
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
