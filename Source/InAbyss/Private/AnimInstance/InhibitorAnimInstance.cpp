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
	// �ǽð����� ȣ��Ǵ� �Լ� (tick����)
	
	// NativeUpdateAnimation �Լ��� BeginPlay �������� ����� (���Ͱ� ���忡 �ö� �������� ����Ǵµ�) 
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
