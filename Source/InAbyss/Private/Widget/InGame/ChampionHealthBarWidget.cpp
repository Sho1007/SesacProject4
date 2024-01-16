// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGame/ChampionHealthBarWidget.h"
#include "Component/StateComponentBase.h"

void UChampionHealthBarWidget::InitStateComponent(UStateComponentBase* NewStateComponent)
{
	Super::InitStateComponent(NewStateComponent);

	NewStateComponent->OnHPChanged.AddUObject(this, &UHealthBarWidgetBase::UpdateHP);
	HPPercent = NewStateComponent->GetHPPercent();
}

void UChampionHealthBarWidget::UpdateMP(float NewMPPercent)
{
	MPPercent = NewMPPercent;
}
