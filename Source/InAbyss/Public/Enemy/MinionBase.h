// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/StateInterface.h"
#include "MinionBase.generated.h"

class USphereComponent;
class UStateComponentBase;

UENUM()
enum class EEnemyState : uint8
{
	NONE,
	IDLE,
	MOVE,
	ATTACK,
	RETURN,	// 경로에서 이탈한 경우
	SIZE
};

class UCapsuleComponent;
UCLASS()
class INABYSS_API AMinionBase : public AActor, public IStateInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EEnemyState GetEnemyState() const;

	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void Separation();
	void Alignment();
	void Cohesion();

	UFUNCTION(CallInEditor)
	void TestDamageFunction();

	// State Interface
	virtual void Damaged() override;
	virtual void Die() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	USphereComponent* SphereComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY()
	class UMinionAnimInstance* AnimInstance;

	UPROPERTY(VisibleInstanceOnly, Category = "State")
	EEnemyState EnemyState;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	UStateComponentBase* StateComponent;

	UPROPERTY(VisibleInstanceOnly, Category = "Target")
	AActor* NextStructure;
	UPROPERTY(VisibleInstanceOnly, Category = "Target")
	AActor* Target;
	UPROPERTY(VisibleInstanceOnly, Category = "Target")
	float TargetDistance;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Move")
	float MoveSpeed = 200.f;

private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<AActor*> NeighborActorArray;
};