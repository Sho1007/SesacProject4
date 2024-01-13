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

public: // ���� ���� ����
	ENexusState  GetNexusState() const;
	
	UPROPERTY(VisibleInstanceOnly, Category = "NexusState")
	ENexusState  NexusState;

public:
	// �ǰݽ� ȣ���� �Լ� = �������� ���� �� ȣ��
	UFUNCTION(CallInEditor)
	void TakeDamage_Nexus();

public: // �������̽� �Լ�
	// �ǰݽ� ü���� ������ �� ȣ��
	virtual void Damaged() override;

	// �ǰݽ� ü���� 0 ������ �� ȣ��
	void Die() override;
};
