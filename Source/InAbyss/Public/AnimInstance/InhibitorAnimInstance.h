// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "InhibitorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API UInhibitorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

//protected:
	UPROPERTY()
	class AInhibitor* Owner;

	UPROPERTY(BlueprintReadWrite) 
	bool bIsDeath = false;


};
