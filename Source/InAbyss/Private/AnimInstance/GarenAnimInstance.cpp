// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/GarenAnimInstance.h"
#include "Character/Garen.h"
#include "Component/StateComponentBase.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/AudioComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "Enemy/MinionBase.h"
#include "Building/Building_Base.h"

void UGarenAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<AGaren>(GetOwningActor());


}

void UGarenAnimInstance::NativeUpdateAnimation(float DeltaSeconds) // 블루프린트에서 애니메이션 전환할 때 필요 
{
	Super::NativeUpdateAnimation(DeltaSeconds);

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
	if (ANM_Idle) {
		StopAllMontages(0.f); // 실행 중이던 모든 몽타주 정지 
		Montage_Play(ANM_Idle);
	}
}

void UGarenAnimInstance::PlayANM_Move()
{
	if (ANM_Move) {

		Montage_Play(ANM_Move);
	}
}

void UGarenAnimInstance::PlayANM_Attack()
{

	if (ANM_Attack_Array.Num() > 0) {
		
		// 일반 공격 중이 아닐 때
		if (bIsAttack_Garen == false) {

			bIsAttack_Garen = true;

			// 스킬 활성화 // 스킬 사용 중이 아니면
			if (bIsSkill_Q == true && bIsSkilling_Q == false) {


				PlayANM_Q();

				Owner->NSComp->Deactivate();

				return;
			}
			else
			{
				Montage_Play(ANM_Attack_Array[IndexNumber]);

				Owner->ADComp->SetSound(Owner->GR_Sounds[IndexNumber]);
				Owner->ADComp->Play();

			}

		}
		// 일반 공격 중일 때 // Q스킬이 활성화 되있고 // 스킬 사용중이 아니라면
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

		Owner->ADComp->SetSound(Owner->GR_Sounds[2]);
		Owner->ADComp->Play();

	}

}
void UGarenAnimInstance::PlayANM_Q()
{
	bIsSkill_Q = false;

	bIsSkilling_Q = true;

	Montage_Play(ANM_Skill_Q);

	Owner->ADComp->SetSound(Owner->GR_SkillSounds[1]);
	Owner->ADComp->Play();

	Owner->VoiceComp->SetSound(Owner->GR_SkillVoice[0]);
	Owner->VoiceComp->Play();

}
void UGarenAnimInstance::PlayANM_E()
{

	if (ANM_Skill_E) {
		
		if (bIsSkilling_E == false) {

			Montage_Play(ANM_Skill_E);

			Owner->ADComp->SetSound(Owner->GR_SkillSounds[3]);
			Owner->ADComp->Play();

			Owner->VoiceComp->SetSound(Owner->GR_SkillVoice[1]);
			Owner->VoiceComp->Play();

			bIsSkilling_E = true;
			
		}

	}
}
void UGarenAnimInstance::PlayANM_R()
{

	if (ANM_Skill_R) {
		
		if (bIsSkilling_R == false) {

			Montage_Play(ANM_Skill_R);
		
			Owner->VoiceComp->SetSound(Owner->GR_SkillVoice[2]);
			Owner->VoiceComp->Play();

			class AActor* SpawnNS = GetWorld()->SpawnActor<AActor>(Owner->NSFactory, Owner->Target_Champion->GetActorLocation() + FVector(0,0,-90), FRotator(0)/*Owner->CurrentRotation*/);
			
			

			bIsSkilling_R = true;
		
		}

	}

}

// 플레이할 몽타주 함수 - 캐릭터에서 호출 ====================================================
// 애니메이션 플레이 시 특정 시점에 호출할 함수 ==============================================
void UGarenAnimInstance::AnimNotify_Attack_Garen()
{
	Owner->Attack_Normal_Garen();

	IndexNumber += 1;

	if (IndexNumber >= ANM_Attack_Array.Num()) {
		IndexNumber = 0;
	}
}

void UGarenAnimInstance::AnimNotify_EndAttack_Garen()
{
	// 공격 애니메이션 호출하는 동안 다시 애니메이션이 호출되지 않고, 끝나면 다시 공격 애니메이션을 호출하도록

	bIsAttack_Garen = false;

}
// ---------------------- Q ------------------------------
void UGarenAnimInstance::AnimNotify_Skill_Q()
{
	//Owner->Attack_Normal_Garen(); // 임시

	if (Owner->Target_Minion) {
		UStateComponentBase* stateComp = Owner->Target_Minion->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(Owner->StateComp_Garen->GetAttackDamage() * 1.3f, Owner->StateComp_Garen->GetAbilityPower() * 1.3f);


	}
	else if (Owner->Target_Champion) {
		UStateComponentBase* stateComp = Owner->Target_Champion->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(Owner->StateComp_Garen->GetAttackDamage() * 1.3f, Owner->StateComp_Garen->GetAbilityPower() * 1.3f);


	}
	else if (Owner->Target_Building) {
		UStateComponentBase* stateComp = Owner->Target_Building->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(Owner->StateComp_Garen->GetAttackDamage() * 1.3f, Owner->StateComp_Garen->GetAbilityPower() * 1.3f);

	}

}

void UGarenAnimInstance::AnimNotify_EndSkill_Q()
{
	// Q 공격 애니메이션이 종료되면 이전에 진행 중이던 공격 애니메이션 이어서 실행

	bIsSkilling_Q = false; // 스킬 사용 중 끄기
	bIsQMove_Garen = false; // Q 이동 끄기
	AnimNotify_EndAttack_Garen();

	Owner->Speed = 300;
}

void UGarenAnimInstance::AnimNotify_Skill_E()
{
	//Owner->Attack_Normal_Garen(); // 임시
	
	
	Owner->E_Skill_Garen();

}

void UGarenAnimInstance::AnimNotify_EndSkill_E()
{
	
	bIsSkilling_E = false;
	bIsSkill_E = false;


}
	

void UGarenAnimInstance::AnimNotify_Skill_R()
{
	// 지정한 대상에게 데미지 - 적 챔피언 한정
	Owner->ADComp->SetSound(Owner->GR_SkillSounds[5]);
	Owner->ADComp->Play();

	//Owner->Attack_Normal_Garen(); // 임시

	if (Owner->Target_Champion) {
		UStateComponentBase* stateComp = Owner->Target_Champion->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(Owner->StateComp_Garen->GetAttackDamage() * 2.f, Owner->StateComp_Garen->GetAbilityPower() * 2.f);

	}

}

void UGarenAnimInstance::AnimNotify_EndSkill_R()
{
	bIsSkill_R = false; // 
	bIsSkilling_R = false; // 


	if (Owner->Target_Champion->GetComponentByClass<UStateComponentBase>()->IsDead() == false) {
		//StopAllMontages(0.f);
		Owner->GarenState = EGarenState::MOVE;

	}

}
