// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

class USkillComponent;
class UChampionAnimInstance;
class UNiagaraSystem;
class UStateComponentBase;
class UInputAction;
class USphereComponent;
struct FInputActionValue;

UENUM()
enum class EChampionState : uint8
{
	NON,
	IDLE,
	ROTATE,
	MOVE,
	ATTACK,
	SKILL,
	MAX
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INABYSS_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSMComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	void FindEnemy();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);

	// Rotate
	bool Rotate(float DeltaTime);
	bool RotateToTarget(float DeltaTime);
	bool RotateToSkillPoint(float DeltaTime);

	// Input
	UFUNCTION()
	void RightClickStarted(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetDestination(FVector NewDestination);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetTarget(AActor* NewTarget);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayAttackAnim();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PlayAttackAnim();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StopMontage();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ApplyDamage();

	bool IsMove() const;
	
	// Attack

	void EndAttack();

	// Skill

	void EndSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_EndSKill();

	void PrepareSkill();
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetSkillPoint(FVector NewSkillPoint);
	
	// UFUNCTION(Server, Reliable)
	// void ServerRPC_RightClickStarted(FVector NewWorldOrigin, FVector NewWorldDirection);

	// Setter
	void SetShouldStop(bool bNewShouldStop);
	// Getter
	EChampionState GetChampionState() const;

	// OnRep
	UFUNCTION()
	void OnRep_Destination();
	UFUNCTION()
	void OnRep_SkillPoint();
	UFUNCTION()
	void OnRep_Target();
	UFUNCTION()
	void OnRep_IsAttacking();
	
private:
	// AnimInstance
	UPROPERTY()
	UChampionAnimInstance* AnimInstance;

	
	// Cursor
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UNiagaraSystem* Cursor;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AActor* CursoredTarget;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	FVector CursoredLocation;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsCursored;

	// State
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	EChampionState ChampionState = EChampionState::IDLE;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	EChampionState BeforeState = EChampionState::IDLE;
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = OnRep_Target, Meta = (AllowPrivateAccess))
	AActor* Target = nullptr;
	UStateComponentBase* TargetStateComponent;
	UPROPERTY(ReplicatedUsing = OnRep_IsAttacking, Meta = (AllowPrivateAccess))
	bool bIsAttacking = false;
	
	// Component
	UPROPERTY(Meta = (AllowPrivateAccess))
	UStateComponentBase* StateComponent;
	UPROPERTY(Meta = (AllowPrivateAccess))
	USkillComponent* SkillComponent;
	
	// Input
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_RightClick;

	// Rotation
	FRotator FromRotation;
	FRotator ToRotation;
	FRotator SkillRotation;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float RotationSpeed = 20.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float RotationTime = 0.f;
	float CurrentRotationTime = 0.f;

	// Skill
	UPROPERTY(ReplicatedUsing = "OnRep_SkillPoint", Meta = (AllowPrivateAccess))
	FVector SkillPoint;

	// Move
	UPROPERTY(ReplicatedUsing = "OnRep_Destination", Meta = (AllowPrivateAccess))
	FVector Destination;
	UPROPERTY(EditDefaultsOnly, Category = "Move", Meta = (AllowPrivateAccess))
	float MoveSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Move", Meta = (AllowPrivateAccess))
	float ReachSuccessDistance = 10.f;

	// GetUnderCursor
	UPROPERTY()
	ACharacter* Owner;
	UPROPERTY()
	APlayerController* PlayerController;
};