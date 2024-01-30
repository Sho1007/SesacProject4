// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Ezreal.generated.h"

class UFSMComponent;
class UStateComponentBase;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UEzrealAnimInstance;
class USkillComponent;
/**
 
 * 
 */

UCLASS()
class INABYSS_API AEzreal : public ACharacterBase
{
	GENERATED_BODY()
public:
	AEzreal();
	
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	
	// Component
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UFSMComponent* FSMComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USkillComponent* SkillComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStateComponentBase* StateComponent;
	
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UCameraComponent* CameraComponent;
	
	
	// Input
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputMappingContext* DefaultIMC;
};