// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTemp/TempInGameGameState.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Widget/UI/ChampionSelectionWidget.h"

void ATempInGameGameState::BeginPlay()
{
	Super::BeginPlay();

    /*
    // 플레이어 컨트롤러 가져오기
    PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        // UI 위젯 생성 및 추가
        ChampionSelectionWidget = CreateWidget<UChampionSelectionWidget>(PC, UChampionSelectionWidget::StaticClass());
        if (ChampionSelectionWidget)
        {
            ChampionSelectionWidget->AddToViewport();
        }
    }
    */
}

void ATempInGameGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATempInGameGameState, ClientsReady);
}


