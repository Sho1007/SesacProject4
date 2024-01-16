// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidgetBase.generated.h"

/**
 * 
 */
class UStateComponentBase;
UCLASS()
class INABYSS_API UHealthBarWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void InitStateComponent(UStateComponentBase* NewStateComponent);

	UFUNCTION()
	void UpdateHP(float NewHPPercent);
	
protected:
	UPROPERTY(BlueprintReadWrite)
	float HPPercent;
};