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

	// Idle����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsIdle_Garen;

	// �̵� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMove_Garen;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack_Garen;
	

};
