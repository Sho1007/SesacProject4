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