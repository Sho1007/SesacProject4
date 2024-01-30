// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/EzrealAnimInstance.h"

#include "Component/SkillComponent.h"
#include "GameFramework/Character.h"

void UEzrealAnimInstance::AnimNotify_Q()
{
	if (Owner->HasAuthority() == false) return;
	Super::AnimNotify_Q();

	SkillComponent->FireQ();
}