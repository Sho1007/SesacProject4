// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/InGame/HealthBarWidgetBase.h"
#include "ChampionHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API UChampionHealthBarWidget : public UHealthBarWidgetBase
{
	GENERATED_BODY()

public:
	virtual void InitStateComponent(UStateComponentBase* NewStateComponent) override;
	void UpdateMP(float NewMPPercent);
	
protected:
	UPROPERTY(BlueprintReadWrite)
	float MPPercent;
};
