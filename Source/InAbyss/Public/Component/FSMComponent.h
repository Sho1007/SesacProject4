// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	

	// Input
	UFUNCTION()
	void RightClickStarted(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RightClickStarted(FVector NewWorldOrigin, FVector NewWorldDirection);

	
	// UFUNCTION(Server, Reliable)
	// void ServerRPC_RightClickStarted(FVector NewWorldOrigin, FVector NewWorldDirection);

	// Setter
	void SetShouldStop(bool bNewShouldStop);
	// Getter
	bool IsMove() const;

	// OnRep
	UFUNCTION()
	void OnRep_Destination();
	
private:
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
	AActor* Target = nullptr;
	bool bIsAttacking = false;
	
	// Component
	UPROPERTY(Meta = (AllowPrivateAccess))
	UStateComponentBase* StateComponent;
	
	// Input
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_RightClick;

	// Rotation
	FRotator FromRotation;
	FRotator ToRotation;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float RotationSpeed = 10.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float RotationTime = 0.f;
	float CurrentRotationTime = 0.f;

	// Move
	UPROPERTY(ReplicatedUsing = "OnRep_Destination", Meta = (AllowPrivateAccess))
	FVector Destination;
	UPROPERTY(EditDefaultsOnly, Category = "Move", Meta = (AllowPrivateAccess))
	float MoveSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Move", Meta = (AllowPrivateAccess))
	float ReachSuccessDistance = 10.f;
	UPROPERTY(Replicated, Meta = (AllowPrivateAccess))
	bool bIsMove = false;
	bool bShouldStop = false;

	// GetUnderCursor
	UPROPERTY()
	ACharacter* Owner;
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	ULocalPlayer* LocalPlayer;
	FVector2D MousePosition;
	FVector WorldOrigin;
	FVector WorldDirection;
};