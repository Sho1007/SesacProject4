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

void UGarenAnimInstance::NativeUpdateAnimation(float DeltaSeconds) // �������Ʈ���� �ִϸ��̼� ��ȯ�� �� �ʿ� 
{
	Super::NativeUpdateAnimation(DeltaSeconds);

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
	if (ANM_Idle) {
		StopAllMontages(0.f); // ���� ���̴� ��� ��Ÿ�� ���� 
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
		
		// �Ϲ� ���� ���� �ƴ� ��
		if (bIsAttack_Garen == false) {

			bIsAttack_Garen = true;

			// ��ų Ȱ��ȭ // ��ų ��� ���� �ƴϸ�
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
		// �Ϲ� ���� ���� �� // Q��ų�� Ȱ��ȭ ���ְ� // ��ų ������� �ƴ϶��
		else if (bIsAttack_Garen == true && bIsSkill_Q == true && bIsSkilling_Q == false) {
			StopAllMontages(0.f);
			PlayANM_Q();
		}

	}
}

void UGarenAnimInstance::PlayANM_Dead()
{
	if (ANM_Dead && Owner->GarenState != EGarenState::DEAD) {
		StopAllMontages(0.f); // ���� ���̴� ��� ��Ÿ�� ���� 
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

// �÷����� ��Ÿ�� �Լ� - ĳ���Ϳ��� ȣ�� ====================================================
// �ִϸ��̼� �÷��� �� Ư�� ������ ȣ���� �Լ� ==============================================
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
	// ���� �ִϸ��̼� ȣ���ϴ� ���� �ٽ� �ִϸ��̼��� ȣ����� �ʰ�, ������ �ٽ� ���� �ִϸ��̼��� ȣ���ϵ���

	bIsAttack_Garen = false;

}
// ---------------------- Q ------------------------------
void UGarenAnimInstance::AnimNotify_Skill_Q()
{
	//Owner->Attack_Normal_Garen(); // �ӽ�

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
	// Q ���� �ִϸ��̼��� ����Ǹ� ������ ���� ���̴� ���� �ִϸ��̼� �̾ ����

	bIsSkilling_Q = false; // ��ų ��� �� ����
	bIsQMove_Garen = false; // Q �̵� ����
	AnimNotify_EndAttack_Garen();

	Owner->Speed = 300;
}

void UGarenAnimInstance::AnimNotify_Skill_E()
{
	//Owner->Attack_Normal_Garen(); // �ӽ�
	
	
	Owner->E_Skill_Garen();

}

void UGarenAnimInstance::AnimNotify_EndSkill_E()
{
	
	bIsSkilling_E = false;
	bIsSkill_E = false;


}
	

void UGarenAnimInstance::AnimNotify_Skill_R()
{
	// ������ ��󿡰� ������ - �� è�Ǿ� ����
	Owner->ADComp->SetSound(Owner->GR_SkillSounds[5]);
	Owner->ADComp->Play();

	//Owner->Attack_Normal_Garen(); // �ӽ�

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
