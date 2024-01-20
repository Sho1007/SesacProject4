// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/EzrealAnimInstance.h"

#include "Character/Ezreal.h"

void UEzrealAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Ezreal = Cast<AEzreal>(GetOwningActor());
}

void UEzrealAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Ezreal == nullptr) return;

	Speed = Ezreal->GetVelocity().Length();
}