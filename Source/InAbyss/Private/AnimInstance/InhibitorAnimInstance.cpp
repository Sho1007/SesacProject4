// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/InhibitorAnimInstance.h"
#include "Building/Inhibitor.h"

void UInhibitorAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<AInhibitor>(GetOwningActor());

}

void UInhibitorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// 실시간으로 호출되는 함수 (tick같음)
	
	// NativeUpdateAnimation 함수는 BeginPlay 이전에도 실행됨 (액터가 월드에 올라간 순간부터 실행되는듯) 
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner && Owner->GetBuildingState() == EBuildingState::Destroy)
	{
		bIsDeath = true;

	}
	else
	{
		bIsDeath = false;
	
	}

}
