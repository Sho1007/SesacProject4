// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ChampionAnimInstance.generated.h"

class UFSMComponent;
/**
 * 
 */
UCLASS()
class INABYSS_API UChampionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	UFUNCTION()
	void AnimNotify_ApplyDamage();
	UFUNCTION()
	void AnimNotify_EndAttack();

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsMove;
	
	UPROPERTY()
	UFSMComponent* FSMComponent;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UAnimMontage* AttackMontage;
	
	ACharacter* Owner;
};