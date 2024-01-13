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

	// �ǹ� �ı��� ȣ���� Ÿ�̸� �Լ�
	void ReBuildTimer();

	// Ÿ�̸Ӱ� �Ϸ�Ǹ� ȣ���� �Լ� 
	void ReBuild();

};

