// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChampionAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "EzrealAnimInstance.generated.h"

/**
 * 
 */
class AEzreal;
UCLASS()
class INABYSS_API UEzrealAnimInstance : public UChampionAnimInstance
{
	GENERATED_BODY()

public:
	virtual void AnimNotify_Q() override;
};