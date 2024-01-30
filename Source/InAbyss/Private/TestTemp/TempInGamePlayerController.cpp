// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTemp/TempInGamePlayerController.h"
#include "Widget/UI/ChampionSelectionWidget.h"
#include "InAbyssGameInstance.h"
#include "GameMode/LoginGameMode.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

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

void ATempInGamePlayerController::CallPlayerNum()
{
	if (GI == nullptr) {
		return;
	}

	
	if (HasAuthority())
	{
		// OnRep_Destination();
		ServerRPCCallPlayerNum_Implementation();
	}
	else
		ServerRPCCallPlayerNum();
	{
	}
}

void ATempInGamePlayerController::ServerRPCCallPlayerNum_Implementation()
{
	ReadyNum++;

	MultiRPCallPlayerNum(ReadyNum);
}

void ATempInGamePlayerController::MultiRPCallPlayerNum_Implementation(int Num)
{
	
	ReadyNum = Num;

	if (GI == nullptr) {
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ReadyNum : %d"), ReadyNum)
	 
	
	if (ChampionSelectionWidget->SelectChampion == false) {
		ChampionSelectionWidget->SelectChampion = true;



		GI->ReadyPlayer = ReadyNum;

		GI->AddReadyPlayer();
	}

}

void ATempInGamePlayerController::ClientRPCCallPlayerNum_Implementation()
{

}

void ATempInGamePlayerController::ActivateStartButton()
{
	ChampionSelectionWidget->UseStartbtn();
}