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
	bool bIsIdle_Garen = false;

	// 이동 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMove_Garen = false;

	// 공격 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack_Garen = false;
	
	// 죽음 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead_Garen = false;

public: // 몽타주 저장할 변수
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Idle;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Move;

	//UPROPERTY(EditDefaultsOnly)
	//class UAnimMontage* ANM_Attack;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Dead;

	// Skill 몽타주
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Skill_Q;

	//UPROPERTY(EditDefaultsOnly)
	//class UAnimMontage* ANM_Skill_W;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Skill_E;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ANM_Skill_R;


	// 몽타주 배열
	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> ANM_Attack_Array;
	int32 AttackMontageIndex = 0;

	// 배열 인덱스
	int IndexNumber = 0;



	bool bIsSkill_Q = false;
	bool bIsSkilling_Q = false;
	bool bIsSkill_W = false;
	bool bIsSkilling_W = false;
	bool bIsSkill_E = false;
	bool bIsSkilling_E = false;
	bool bIsSkill_R = false;
	bool bIsSkilling_R = false;


public: // 몽타주 플레이 함수
	void PlayANM_Idle();
	void PlayANM_Move();
	void PlayANM_Attack();
	void PlayANM_Dead();

	// 스킬 몽타주
	void PlayANM_Q();
	//void PlayANM_W();
	void PlayANM_E();
	void PlayANM_R();



public: // AnimNotify - 애니메이션 특정 시점에서 Garen클래스의 함수를 호출 
	UFUNCTION()
	void AnimNotify_Attack_Garen();
	UFUNCTION()
	void AnimNotify_EndAttack_Garen();

	// 스킬
	UFUNCTION()
	void AnimNotify_Skill_Q();

	UFUNCTION()
	void AnimNotify_EndSkill_Q();

};
