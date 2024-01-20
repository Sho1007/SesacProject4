// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/GarenAnimInstance.h"
#include "Character/Garen.h"

void UGarenAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<AGaren>(GetOwningActor());


}

void UGarenAnimInstance::NativeUpdateAnimation(float DeltaSeconds) // 블루프린트에서 애니메이션 전환할 때 필요 
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 상태는 중복될 수 없으므로, 하나의 변수만 true일 것임

	// 기본
	if (Owner && Owner->GarenState == EGarenState::IDLE) {
		// 기본 애니메이션 
		bIsIdle_Garen = true;
	}
	else
	{
		bIsIdle_Garen = false;
	}
	
	// 이동
	if (Owner && Owner->GarenState == EGarenState::MOVE) {
		// 이동 애니메이션 
		bIsMove_Garen = true;
	}
	else
	{
		bIsMove_Garen = false;
	}

	// 공격
	if (Owner && Owner->GarenState == EGarenState::ATTACK) {
		// 공격 애니메이션 
		bIsAttack_Garen = true;
	}
	else
	{
		bIsAttack_Garen = false;
	}

	// 죽음
	if (Owner && Owner->GarenState == EGarenState::DEAD) {
		// 죽음 애니메이션 
		bIsDead_Garen = true;
	}
	else
	{
		bIsDead_Garen = false;
	}


}
// 플레이할 몽타주 함수 - 캐릭터에서 호출 ====================================================
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
		StopAllMontages(0.f); // 실행 중이던 모든 몽타주 정지 
		Montage_Play(ANM_Dead);
	}
	
}
// 플레이할 몽타주 함수 - 캐릭터에서 호출 ====================================================
// 애니메이션 플레이 시 특정 시점에 호출할 함수 ==============================================
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
	// 공격 애니메이션 호출하는 동안 다시 애니메이션이 호출되지 않고, 끝나면 다시 공격 애니메이션을 호출하도록
}
