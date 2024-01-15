// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Building_Base.h"
#include "Nexus.generated.h"


/**
 * 
 */
UCLASS()
class INABYSS_API ANexus : public ABuilding_Base
{
	GENERATED_BODY()
	
public:
	ANexus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: // 건축물의 순서를 정하기 위한 기능
	// 모든 레벨의 건축물을 저장하는 기능
	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> AllBuilding;

	void ScanBuildingInLevel();


public:
	// 피격시 호출할 함수 = 데미지를 입을 때 호출
	UFUNCTION(CallInEditor)
	void TakeDamage_Nexus();

public: // 인터페이스 함수
	// 피격시 체력이 남았을 때 호출
	virtual void Damaged() override;

	// 피격시 체력이 0 이하일 때 호출
	void Die() override;
};
