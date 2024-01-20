// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GarenAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INABYSS_API UGarenAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
protected:
	// Player
	UPROPERTY()
	class AGaren* Owner;

	// Idle변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsIdle_Garen;

	// 이동 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMove_Garen;

	// 공격 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack_Garen;
	
	// 죽음 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead_Garen;

public: // 몽타주
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Idle;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Move;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Attack;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Dead;

public: // 몽타주 플레이 함수
	void PlayANM_Idle();
	void PlayANM_Move();
	void PlayANM_Attack();
	void PlayANM_Dead();

public: // AnimNotify - 애니메이션 특정 시점에서 Garen클래스의 함수를 호출 
	void AnimNotify_Attack1_Garen();
	void AnimNotify_Attack2_Garen();
	void AnimNotify_EndAttack_Garen();

};
