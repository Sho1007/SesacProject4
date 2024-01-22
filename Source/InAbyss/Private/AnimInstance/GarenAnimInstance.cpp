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

	/*
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

	*/

	UE_LOG(LogTemp, Warning, TEXT("bIsSkilling_Q is: %d"), bIsSkilling_Q);
	UE_LOG(LogTemp, Warning, TEXT("bIsSkill_Q is: %d"), bIsSkill_Q );


}
// 플레이할 몽타주 함수 - 캐릭터에서 호출 ====================================================
void UGarenAnimInstance::PlayANM_Idle()
{
	if (ANM_Idle) {
		StopAllMontages(0.f); // 실행 중이던 모든 몽타주 정지 
		Montage_Play(ANM_Idle);
	}
}

void UGarenAnimInstance::PlayANM_Move()
{
	if (ANM_Move) {

		//StopAllMontages(0.f); // 실행 중이던 모든 몽타주 정지 
		Montage_Play(ANM_Move);
	}
}

void UGarenAnimInstance::PlayANM_Attack()
{

	if (ANM_Attack_Array.Num() > 0) {
		//StopAllMontages(0.f); // 실행 중이던 모든 몽타주 정지 

		
		// 일반 공격 중이 아닐 때
		if (bIsAttack_Garen == false) {

			bIsAttack_Garen = true;

			// 스킬 활성화 // 스킬 사용 중이 아니면
			if (bIsSkill_Q == true && bIsSkilling_Q == false) {


				PlayANM_Q();

				return;
			}
			else
			{
				Montage_Play(ANM_Attack_Array[IndexNumber]);

				IndexNumber += 1;
			}
			

		}
		// 일반 공격 중일 때 // 스킬이 활성화 되있고 // 스킬 사용중이 아니라면
		else if (bIsAttack_Garen == true && bIsSkill_Q == true && bIsSkilling_Q == false) {
			StopAllMontages(0.f);
			PlayANM_Q();
		}

	}
}

void UGarenAnimInstance::PlayANM_Dead()
{
	if (ANM_Dead && Owner->GarenState != EGarenState::DEAD) {
		StopAllMontages(0.f); // 실행 중이던 모든 몽타주 정지 
		Montage_Play(ANM_Dead);
	}

}
void UGarenAnimInstance::PlayANM_Q()
{
	bIsSkill_Q = false;

	bIsSkilling_Q = true;

	Montage_Play(ANM_Skill_Q);
}
void UGarenAnimInstance::PlayANM_E()
{

	if (ANM_Skill_E) {
		Montage_Play(ANM_Skill_E);
	}
}
void UGarenAnimInstance::PlayANM_R()
{

	if (ANM_Skill_R) {
		Montage_Play(ANM_Skill_R);

	}
}

// 플레이할 몽타주 함수 - 캐릭터에서 호출 ====================================================
// 애니메이션 플레이 시 특정 시점에 호출할 함수 ==============================================
void UGarenAnimInstance::AnimNotify_Attack_Garen()
{
	Owner->Attack_Normal_Garen();
}

void UGarenAnimInstance::AnimNotify_EndAttack_Garen()
{
	// 공격 애니메이션 호출하는 동안 다시 애니메이션이 호출되지 않고, 끝나면 다시 공격 애니메이션을 호출하도록

	//UE_LOG(LogTemp, Warning, TEXT("CALLCALLCALLCALLCALL"));

	if (IndexNumber >= ANM_Attack_Array.Num()) {
		IndexNumber = 0;
	}

	bIsAttack_Garen = false;

}
// ---------------------- Q ------------------------------
void UGarenAnimInstance::AnimNotify_Skill_Q()
{
	Owner->Attack_Normal_Garen(); // 임시

}

void UGarenAnimInstance::AnimNotify_EndSkill_Q()
{
	// Q 공격 애니메이션이 종료되면 이전에 진행 중이던 공격 애니메이션 이어서 실행

	bIsSkilling_Q = false;
	AnimNotify_EndAttack_Garen();
}
