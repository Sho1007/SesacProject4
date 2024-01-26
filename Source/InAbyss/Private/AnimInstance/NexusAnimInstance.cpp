// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/NexusAnimInstance.h"
#include "Building/Nexus.h"

void UNexusAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ANexus>(GetOwningActor());

}

void UNexusAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner && Owner->BuildingState == EBuildingState::Destroy) {
		bIsDeath = true;
	}

}
