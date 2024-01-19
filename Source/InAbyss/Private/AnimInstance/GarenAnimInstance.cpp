// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/GarenAnimInstance.h"
#include "Character/Garen.h"

void UGarenAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<AGaren>(GetOwningActor());


}

void UGarenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner && Owner->GarenState == EGarenState::IDLE) {
		// �̵� �ִϸ��̼� 
		bIsIdle_Garen = true;
	}
	else
	{
		bIsIdle_Garen = false;
	}
	
	if (Owner && Owner->GarenState == EGarenState::MOVE) {
		// �̵� �ִϸ��̼� 
		bIsMove_Garen = true;
	}
	else
	{
		bIsMove_Garen = false;
	}

	if (Owner && Owner->GarenState == EGarenState::ATTACK) {
		// �̵� �ִϸ��̼� 
		bIsAttack_Garen = true;
	}
	else
	{
		bIsAttack_Garen = false;
	}




}
