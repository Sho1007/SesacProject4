// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Ezreal.generated.h"

class UStateComponentBase;
class UCameraComponent;
class USpringArmComponent;
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
	AEzreal();
	
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void Rotate();


	UFUNCTION(Server, Reliable)
	void ServerRPC_RightClickStarted(FVector WorldOrigin, FVector WorldDirection);
	
	UFUNCTION()
	void RightClickStarted(const FInputActionValue& Value);

	// Getter
	bool IsMove() const;

	// OnRep
	UFUNCTION()
	void OnRep_ActorRotation();
private:
	// Rotation
	UPROPERTY(ReplicatedUsing = "OnRep_ActorRotation", Meta = (AllowPrivateAccess))
	FRotator ActorRotation;
	// Move
	FVector Destination;
	UPROPERTY(EditDefaultsOnly, Category = "Move", Meta = (AllowPrivateAccess))
	float MoveSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Move", Meta = (AllowPrivateAccess))
	float ReachSuccessDistance = 10.f;
	UPROPERTY(Replicated, Meta = (AllowPrivateAccess))
	bool bIsMove = false;
	
	// Component
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UCameraComponent* CameraComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStateComponentBase* StateComponent;
	
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
