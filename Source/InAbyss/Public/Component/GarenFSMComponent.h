// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/FSMComponent.h"
#include "GarenFSMComponent.generated.h"

//UENUM()
//enum class EGarenState: uint8
//{
//	NONE,
//	IDLE,
//	MOVE,
//	ATTACK,
//	SKILL,
//	DEAD
//};

UCLASS()
class INABYSS_API UGarenFSMComponent : public UFSMComponent
{
	GENERATED_BODY()
	/*
public:
	UGarenFSMComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_RightClick;





	// GetUnderCursor
	UPROPERTY()
	class AGaren* Owner;
	UPROPERTY()
	class APlayerController* PlayerController;


	// Component
	UPROPERTY(Meta = (AllowPrivateAccess))
	class UStateComponentBase* StateComponent;
	UPROPERTY(Meta = (AllowPrivateAccess))
	class USkillComponent* SkillComponent;
	


public: // 가렌 행동 상태 
	//EGarenState GetGarenState() const;

	UPROPERTY(EditAnywhere)
	EGarenState GarenState;



public: // 애니메이션
	UPROPERTY()
	class UGarenAnimInstance* GarenAnim;





	*/
};
