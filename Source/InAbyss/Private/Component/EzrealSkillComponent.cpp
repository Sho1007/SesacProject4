// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EzrealSkillComponent.h"

#include "Projectile/EzrealQ.h"

void UEzrealSkillComponent::FireQ()
{
	Super::FireQ();

	FActorSpawnParameters Params;
	
	if (EzrealQClass)
	{
		AEzrealQ* EzrealQ =  GetWorld()->SpawnActor<AEzrealQ>(EzrealQClass,
			GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(),
			Params);
	}
}
