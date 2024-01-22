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
	bool bIsIdle_Garen = false;

	// �̵� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMove_Garen = false;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack_Garen = false;
	
	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead_Garen = false;

public: // ��Ÿ�� ������ ����
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Idle;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Move;

	//UPROPERTY(EditDefaultsOnly)
	//class UAnimMontage* ANM_Attack;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Dead;

	// Skill ��Ÿ��
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Skill_Q;

	//UPROPERTY(EditDefaultsOnly)
	//class UAnimMontage* ANM_Skill_W;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Skill_E;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Skill_R;


	// ��Ÿ�� �迭
	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> ANM_Attack_Array;
	int32 AttackMontageIndex = 0;

	// �迭 �ε���
	int IndexNumber = 0;



	bool bIsSkill_Q = false;
	bool bIsSkilling_Q = false;
	bool bIsSkill_W = false;
	bool bIsSkilling_W = false;
	bool bIsSkill_E = false;
	bool bIsSkilling_E = false;
	bool bIsSkill_R = false;
	bool bIsSkilling_R = false;


public: // ��Ÿ�� �÷��� �Լ�
	void PlayANM_Idle();
	void PlayANM_Move();
	void PlayANM_Attack();
	void PlayANM_Dead();

	// ��ų ��Ÿ��
	void PlayANM_Q();
	//void PlayANM_W();
	void PlayANM_E();
	void PlayANM_R();



public: // AnimNotify - �ִϸ��̼� Ư�� �������� GarenŬ������ �Լ��� ȣ�� 
	UFUNCTION()
	void AnimNotify_Attack_Garen();
	UFUNCTION()
	void AnimNotify_EndAttack_Garen();

	// ��ų
	UFUNCTION()
	void AnimNotify_Skill_Q();

	UFUNCTION()
	void AnimNotify_EndSkill_Q();

};
