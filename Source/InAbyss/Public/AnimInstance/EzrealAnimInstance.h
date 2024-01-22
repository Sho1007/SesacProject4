// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EzrealAnimInstance.generated.h"

class UFSMComponent;
/**
 * 
 */
class AEzreal;
UCLASS()
class INABYSS_API UEzrealAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_QStart();

	void SetQReady();
private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsMove;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsQReady;
	UPROPERTY()
	AEzreal* Ezreal;
	UPROPERTY()
	UFSMComponent* FSMComponent;
};