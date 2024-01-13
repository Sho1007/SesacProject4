// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Building_Base.h"
#include "Turret_Base.generated.h"


UENUM()
enum class ETurretState : uint8
{
	NONE,
	IDLE,
	ATTACK,
	Destroy
};

/**
 * 
 */
UCLASS()
class INABYSS_API ATurret_Base : public ABuilding_Base
{
	GENERATED_BODY()
	
public:
	ATurret_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;

public: // ���� ���� ����
	ETurretState GetTurretState() const;
	
	UPROPERTY(VisibleInstanceOnly, Category = "TurretState")
	ETurretState TurretState;

public:
	// ��ž �ֺ��� è�ǿ� �� �̴Ͼ��� ������ Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* DetectCollisionComp;
	
	// ���� ���� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* AttackStartPointComp;

public: // �������

	// ������ ������ ������ �迭
	TArray <AActor*> DetectTargets_Test; // ���߿� �ʿ����

	TArray <AActor*> DetectTargets_EnemyChampion; // �� è�Ǿ� ������ �迭
	TArray <AActor*> DetectTargets_SuperOrCanon; // ���� Ȥ�� ���� �̴Ͼ� ������ �迭
	TArray <AActor*> DetectTargets_Warrior; // ���� �̴Ͼ� ������ �迭
	TArray <AActor*> DetectTargets_Wizard; // ������ �̴Ͼ� ������ �迭

	// ������ ����� ������ ����
	class AActor* CurrentTarget; // ���� ���� Ÿ�� ����
	class AActor* TopPriorityTarget; // CurrentTarget���� ���� ���ݹ��� Ÿ�� ���� // ���߿� Ŭ������ è�Ǿ����� ����

public:
	// ���ݴ�� ������ ��� 
	void RetargetCurrentTarget();

public:
	// ���ݱ��
	void Attact_SpawnProjectile();

	// ���� �ð����� ����� ����
	float CurrentTIme;
	float TargetTIme = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectile_Turret> ProjectileFactory;

public:
	// �ǰݽ� ȣ���� �Լ� = �������� ���� �� ȣ��
	UFUNCTION(CallInEditor)
	void TakeDamage_Turret();

public: // �������̽� �Լ�
	// �ǰݽ� ü���� ������ �� ȣ��
	virtual void Damaged() override;

	// �ǰݽ� ü���� 0 ������ �� ȣ��
	void Die() override;
};


