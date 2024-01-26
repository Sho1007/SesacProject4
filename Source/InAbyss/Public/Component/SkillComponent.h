// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"


class UFSMComponent;
class UInputAction;
class UEzrealAnimInstance;
struct FInputActionValue;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INABYSS_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();
	
	void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);

	UFUNCTION()
	void QStarted(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void ServerRPC_Q();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Q();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Animation
	UEzrealAnimInstance* AnimInstance;

	// Other Component
	UFSMComponent* FSMComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_Q;
};
