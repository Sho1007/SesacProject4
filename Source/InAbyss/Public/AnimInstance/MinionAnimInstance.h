// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MinionAnimInstance.generated.h"

class AMinionBase;
UCLASS()
class INABYSS_API UMinionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void PlayDeathMontage();

	
protected:
	UPROPERTY()
	AMinionBase* Owner;
	UPROPERTY(BlueprintReadOnly) 
	bool bIsMove = false;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> AttackMontageArray;
	int32 AttackMontageIndex = 0;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

	UFUNCTION()
	void AnimNotify_OnDeathFinished();
	UFUNCTION()
	void AnimNotify_Attack();
	UFUNCTION()
	void AnimNotify_EndAttack();
};