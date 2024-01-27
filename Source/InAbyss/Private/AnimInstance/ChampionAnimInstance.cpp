// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/ChampionAnimInstance.h"

#include "Component/FSMComponent.h"
#include "GameFramework/Character.h"
#include "InAbyss/InAbyss.h"

void UChampionAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACharacter>(GetOwningActor());
	FSMComponent = Owner->GetComponentByClass<UFSMComponent>();
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

void UChampionAnimInstance::AnimNotify_ApplyDamage()
{
	if (Owner->IsLocallyControlled())
	{
		FSMComponent->ServerRPC_ApplyDamage();
	}
}

void UChampionAnimInstance::AnimNotify_EndAttack()
{
	FSMComponent->EndAttack();
}
