// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTemp/TempInGamePlayerController.h"
#include "Widget/UI/ChampionSelectionWidget.h"
#include "InAbyssGameInstance.h"
#include "GameMode/LoginGameMode.h"
#include <GameFramework/PlayerState.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

ATempInGamePlayerController::ATempInGamePlayerController()
{



}

void ATempInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);


	if (IsLocalController() == false) return;


	GI = Cast<UInAbyssGameInstance>(GetWorld()->GetGameInstance());

	if (ChampionSelectionWidgetClass)
	{
		ChampionSelectionWidget = CreateWidget<UChampionSelectionWidget>(this, ChampionSelectionWidgetClass);
		ChampionSelectionWidget->AddToViewport();
	}
}

void ATempInGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 동기화할 변수 등록
	DOREPLIFETIME(ATempInGamePlayerController, ReadyNum);

}

void ATempInGamePlayerController::ServerRPC_SetPlayerChampion_Implementation(const FString& NewPlayerChampion)
{
	GetGameInstance<UInAbyssGameInstance>()->SetPlayerChampion(FName(PlayerState->GetPlayerName()), NewPlayerChampion);
}

void ATempInGamePlayerController::CallPlayerNum()
{
	if (GI == nullptr) {
		return;
	}

	ServerRPCCallPlayerNum();

	/*if (HasAuthority())
	{
		ServerRPCCallPlayerNum_Implementation();
	}
	else
	{
	}*/
}

void ATempInGamePlayerController::ServerRPCCallPlayerNum_Implementation()
{
	ReadyNum++;

	// 모든 컨트롤러의 Client RPC 호출하기
	for (TActorIterator<ATempInGamePlayerController> it(GetWorld()); it; ++it)
	{
		auto pc = *it;
		pc->ClientRPCCallPlayerNum(ReadyNum);
	}

	UE_LOG(LogTemp, Warning, TEXT(" Success1: %d"), ReadyNum)

}

void ATempInGamePlayerController::MultiRPCallPlayerNum_Implementation(int Num)
{


}

void ATempInGamePlayerController::ClientRPCCallPlayerNum_Implementation(int Num)
{
	ReadyNum = Num;

	if (GI == nullptr) {
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ReadyNum : %d"), ReadyNum);





	GI->ReadyPlayer = ReadyNum;

	GI->AddReadyPlayer();

}

void ATempInGamePlayerController::ActivateStartButton()
{
	ChampionSelectionWidget->UseStartbtn();
}

void ATempInGamePlayerController::OnRep_ReadyNum()
{
	//ReadyNum = Num;

	if (GI == nullptr) {
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ReadyNum : %d"), ReadyNum);





	GI->ReadyPlayer = ReadyNum;

	GI->AddReadyPlayer();

}
