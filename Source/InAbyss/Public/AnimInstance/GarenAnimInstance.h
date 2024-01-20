// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GarenAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API UGarenAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
protected:
	// Player
	UPROPERTY()
	class AGaren* Owner;

	// Idle����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsIdle_Garen;

	// �̵� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMove_Garen;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack_Garen;
	
	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead_Garen;

public: // ��Ÿ��
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Idle;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Move;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Attack;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Dead;

public: // ��Ÿ�� �÷��� �Լ�
	void PlayANM_Idle();
	void PlayANM_Move();
	void PlayANM_Attack();
	void PlayANM_Dead();

public: // AnimNotify - �ִϸ��̼� Ư�� �������� GarenŬ������ �Լ��� ȣ�� 
	void AnimNotify_Attack1_Garen();
	void AnimNotify_Attack2_Garen();
	void AnimNotify_EndAttack_Garen();

};
