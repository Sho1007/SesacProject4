// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/SkillComponent.h"
#include "EzrealSkillComponent.generated.h"

class AEzrealQ;
/**
 * 
 */
UCLASS()
class INABYSS_API UEzrealSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	virtual void FireQ() override;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<AEzrealQ> EzrealQClass;
};