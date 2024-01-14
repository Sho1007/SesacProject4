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

public: // ���� ���� ����
	EInhibitorState   GetInhibitorState() const;
	
	UPROPERTY(VisibleInstanceOnly, Category = "InhibitorState")
	EInhibitorState   InhibitorState;

public:
	// �ǰݽ� ȣ���� �Լ� = �������� ���� �� ȣ��
	UFUNCTION(CallInEditor)
	void TakeDamage_Inhibitor();

public:	// 
	// �ǹ� �ı��� ȣ���� Ÿ�̸� �Լ�
	void ReBuildTimer();

	// Ÿ�̸Ӱ� �Ϸ�Ǹ� ȣ���� �Լ� 
	void ReBuild();

public: // �������̽� �Լ�
	// �ǰݽ� ü���� ������ �� ȣ��
	virtual void Damaged() override;

	// �ǰݽ� ü���� 0 ������ �� ȣ��
	void Die() override;

public: // �ִϸ��̼�
	UPROPERTY()
	class UInhibitorAnimInstance* InhibitorAnimInstance;
	

};

