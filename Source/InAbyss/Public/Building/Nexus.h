// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Building_Base.h"
#include "Nexus.generated.h"


UENUM()
enum class ENexusState : uint8
{
	NONE,
	IDLE,
	Destroy
};

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

public: // 상태 관련 설정
	ENexusState  GetNexusState() const;
	
	UPROPERTY(VisibleInstanceOnly, Category = "NexusState")
	ENexusState  NexusState;

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
