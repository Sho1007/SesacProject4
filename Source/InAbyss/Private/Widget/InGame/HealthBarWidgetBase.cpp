// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGame/HealthBarWidgetBase.h"

#include "Component/StateComponentBase.h"

void UHealthBarWidgetBase::InitStateComponent(UStateComponentBase* NewStateComponent)
{
	NewStateComponent->OnHPChanged.AddUObject(this, &UHealthBarWidgetBase::UpdateHP);
	HPPercent = NewStateComponent->GetHPPercent();
}

void UHealthBarWidgetBase::UpdateHP(float NewHPPercent)
{
	HPPercent = NewHPPercent;
}