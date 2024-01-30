// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"


class UFSMComponent;
class UInputAction;
class UChampionAnimInstance;
struct FInputActionValue;
UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INABYSS_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();
	
	void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);

	UFUNCTION()
	void QStarted(const FInputActionValue& Value);
	// 스킬 모션 시작
	void Q();
	// 실제 스킬 작동
	virtual void FireQ();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Q();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Q();

	UFUNCTION(Server,Reliable)
	void ServerRPC_EndSkill();

	// OnRep
	UFUNCTION()
	void OnRep_IsSkilling();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getter
	bool IsSkilling() const;
	
private:
	// Animation
	UChampionAnimInstance* AnimInstance;

	// Other Component
	UFSMComponent* FSMComponent;

	UPROPERTY(ReplicatedUsing = OnRep_IsSkilling, EditDefaultsOnly, Category = "Skill", Meta = (AllowPrivateAccess))
	bool bIsSkilling;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_Q;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_W;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_E;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_R;
};