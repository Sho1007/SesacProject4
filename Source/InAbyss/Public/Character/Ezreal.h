// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Ezreal.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class INABYSS_API AEzreal : public ACharacterBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void Rotate();

	UFUNCTION()
	void RightClickStarted(const FInputActionValue& Value);
private:
	// Input
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputMappingContext* DefaultIMC;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_RightClick;

	bool bIsRightClicked = false;

	// Rotation
	FTimerHandle RotationTimerHandle;
	FRotator FromRotation;
	FRotator ToRotation;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float RotationTime = 1.f;
};
