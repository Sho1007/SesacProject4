// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/MinionAnimInstance.h"

#include "Enemy/MinionBase.h"

void UMinionAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<AMinionBase>(GetOwningActor());
}

void UMinionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// NativeUpdateAnimation 함수는 BeginPlay 이전에도 실행됨 (액터가 월드에 올라간 순간부터 실행되는듯) 
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (Owner && Owner->GetEnemyState() == EEnemyState::MOVE)
	{
		// UE_LOG(LogTemp, Warning, TEXT("UMinionAnimInstance::NativeUpdateAnimation) Move is True"));
		bIsMove = true;
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("UMinionAnimInstance::NativeUpdateAnimation) Move is False"));
		bIsMove = false;
	}
}

void UMinionAnimInstance::PlayAttackMontage()
{
	if (AttackMontageArray.Num() == 0) return;
	
	Montage_Play(AttackMontageArray[AttackMontageIndex]);

	AttackMontageIndex = (AttackMontageIndex + 1) % AttackMontageArray.Num(); 
}

void UMinionAnimInstance::PlayDeathMontage()
{
	if (DeathMontage)
	{
		StopAllMontages(0.f);
		Montage_Play(DeathMontage);
	}
}

void UMinionAnimInstance::AnimNotify_OnDeathFinished()
{
	if (GetOwningActor()->HasAuthority() == false) return;

	Owner->Deactivate();
}

void UMinionAnimInstance::AnimNotify_Attack()
{
	if (GetOwningActor()->HasAuthority() == false) return;

	Owner->Attack();
}

void UMinionAnimInstance::AnimNotify_EndAttack()
{
	if (GetOwningActor()->HasAuthority() == false) return;
	Owner->EndAttack();
}