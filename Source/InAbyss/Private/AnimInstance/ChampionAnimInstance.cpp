// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/ChampionAnimInstance.h"

#include "Component/FSMComponent.h"
#include "Component/SkillComponent.h"
#include "GameFramework/Character.h"
#include "InAbyss/InAbyss.h"

void UChampionAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACharacter>(GetOwningActor());
	FSMComponent = Owner->GetComponentByClass<UFSMComponent>();
	SkillComponent = Owner->GetComponentByClass<USkillComponent>();
}

void UChampionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (FSMComponent)
	{
		bIsMove = FSMComponent->IsMove();
	}
}

void UChampionAnimInstance::PlayAttackMontage()
{
	if (AttackMontage)
	{
		Montage_Play(AttackMontage);
	}
}

void UChampionAnimInstance::PlayQMontage()
{
	if (QMontage)
	{
		Montage_Play(QMontage);
	}
}

void UChampionAnimInstance::PlayWMontage()
{
	if (WMontage)
	{
		Montage_Play(WMontage);
	}
}

void UChampionAnimInstance::PlayEMontage()
{
	if (EMontage)
	{
		Montage_Play(EMontage);
	}
}

void UChampionAnimInstance::PlayRMontage()
{
	if (RMontage)
	{
		Montage_Play(RMontage);
	}
}

void UChampionAnimInstance::AnimNotify_ApplyDamage()
{
	if (Owner->IsLocallyControlled())
	{
		FSMComponent->ServerRPC_ApplyDamage();
	}
}

void UChampionAnimInstance::AnimNotify_Q()
{
}

void UChampionAnimInstance::AnimNotify_EndAttack()
{
	FSMComponent->EndAttack();
}

void UChampionAnimInstance::AnimNotify_EndSkill()
{
	if (Owner->IsLocallyControlled())
	{
		SkillComponent->ServerRPC_EndSkill();
	}
}