// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/GarenAnimInstance.h"
#include "Character/Garen.h"

void UGarenAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<AGaren>(GetOwningActor());


}

void UGarenAnimInstance::NativeUpdateAnimation(float DeltaSeconds) // �������Ʈ���� �ִϸ��̼� ��ȯ�� �� �ʿ� 
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ���´� �ߺ��� �� �����Ƿ�, �ϳ��� ������ true�� ����

	// �⺻
	if (Owner && Owner->GarenState == EGarenState::IDLE) {
		// �⺻ �ִϸ��̼� 
		bIsIdle_Garen = true;
	}
	else
	{
		bIsIdle_Garen = false;
	}
	
	// �̵�
	if (Owner && Owner->GarenState == EGarenState::MOVE) {
		// �̵� �ִϸ��̼� 
		bIsMove_Garen = true;
	}
	else
	{
		bIsMove_Garen = false;
	}

	// ����
	if (Owner && Owner->GarenState == EGarenState::ATTACK) {
		// ���� �ִϸ��̼� 
		bIsAttack_Garen = true;
	}
	else
	{
		bIsAttack_Garen = false;
	}

	// ����
	if (Owner && Owner->GarenState == EGarenState::DEAD) {
		// ���� �ִϸ��̼� 
		bIsDead_Garen = true;
	}
	else
	{
		bIsDead_Garen = false;
	}


}
// �÷����� ��Ÿ�� �Լ� - ĳ���Ϳ��� ȣ�� ====================================================
void UGarenAnimInstance::PlayANM_Idle()
{
	Montage_Play(ANM_Idle);
}

void UGarenAnimInstance::PlayANM_Move()
{
	Montage_Play(ANM_Move);
}

void UGarenAnimInstance::PlayANM_Attack()
{
	if (ANM_Attack) {
		Montage_Play(ANM_Attack);
	}
}

void UGarenAnimInstance::PlayANM_Dead()
{
	if (ANM_Dead && Owner->GarenState != EGarenState::DEAD) {
		StopAllMontages(0.f); // ���� ���̴� ��� ��Ÿ�� ���� 
		Montage_Play(ANM_Dead);
	}
	
}
// �÷����� ��Ÿ�� �Լ� - ĳ���Ϳ��� ȣ�� ====================================================
// �ִϸ��̼� �÷��� �� Ư�� ������ ȣ���� �Լ� ==============================================
void UGarenAnimInstance::AnimNotify_Attack1_Garen()
{
	Owner->Attack_Normal_Garen();
}

void UGarenAnimInstance::AnimNotify_Attack2_Garen()
{
	Owner->Attack_Normal_Garen();
}

void UGarenAnimInstance::AnimNotify_EndAttack_Garen()
{
	// ���� �ִϸ��̼� ȣ���ϴ� ���� �ٽ� �ִϸ��̼��� ȣ����� �ʰ�, ������ �ٽ� ���� �ִϸ��̼��� ȣ���ϵ���
}
