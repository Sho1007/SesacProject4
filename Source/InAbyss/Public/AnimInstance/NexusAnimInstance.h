// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NexusAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API UNexusAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

//protected:
	UPROPERTY()
	class ANexus* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bIsDeath = false;

};
