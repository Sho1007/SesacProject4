// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Building_Base.h"
#include "Inhibitor.generated.h"

UENUM()
enum class EInhibitorState : uint8
{
	NONE,
	IDLE,
	Destroy
};

/**
 * 
 */
UCLASS()
class INABYSS_API AInhibitor : public ABuilding_Base
{
	GENERATED_BODY()
	
public:
	AInhibitor();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: // 상태 관련 설정
	EInhibitorState   GetInhibitorState() const;
	
	UPROPERTY(VisibleInstanceOnly, Category = "InhibitorState")
	EInhibitorState   InhibitorState;

public:
	// 피격시 호출할 함수 = 데미지를 입을 때 호출
	UFUNCTION(CallInEditor)
	void TakeDamage_Inhibitor();

public:	// 
	// 건물 파괴시 호출할 타이머 함수
	void ReBuildTimer();

	// 타이머가 완료되면 호출할 함수 
	void ReBuild();

public: // 인터페이스 함수
	// 피격시 체력이 남았을 때 호출
	virtual void Damaged() override;

	// 피격시 체력이 0 이하일 때 호출
	void Die() override;

public: // 애니메이션
	UPROPERTY()
	class UInhibitorAnimInstance* InhibitorAnimInstance;
	

};

