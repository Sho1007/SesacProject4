// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Building_Base.h"
#include "Inhibitor.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API AInhibitor : public ABuilding_Base
{
	GENERATED_BODY()
	
public:
	AInhibitor();

	// 건물 파괴시 호출할 타이머 함수
	void ReBuildTimer();

	// 타이머가 완료되면 호출할 함수 
	void ReBuild();

};

