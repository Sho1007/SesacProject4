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
	

};
