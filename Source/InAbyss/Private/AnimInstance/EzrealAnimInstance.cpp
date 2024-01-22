// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/EzrealAnimInstance.h"

#include "Character/Ezreal.h"
#include "Component/FSMComponent.h"

void UEzrealAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Ezreal = Cast<AEzreal>(GetOwningActor());
	FSMComponent = Ezreal->GetComponentByClass<UFSMComponent>();
}

void UEzrealAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Ezreal == nullptr) return;

	bIsMove = FSMComponent->IsMove();
}

void UEzrealAnimInstance::AnimNotify_QStart()
{
	bIsQReady = false;
}

void UEzrealAnimInstance::SetQReady()
{
	bIsQReady = true;
}
