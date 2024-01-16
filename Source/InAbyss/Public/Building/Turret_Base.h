// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Building_Base.h"
#include "Turret_Base.generated.h"


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

public:
	// ��ž �ֺ��� è�ǿ� �� �̴Ͼ��� ������ Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* DetectCollisionComp;
	
	// ���� ���� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* AttackStartPointComp;

public: // �������

	// ������ ������ ������ �迭
	// �ӽ�---------------
	//TArray <AActor*> DetectTargets_Test; // ���߿� �ʿ����

	//TArray <AActor*> DetectTargets_Chracter_Test; // �� è�Ǿ� ������ �迭
	//TArray <AActor*> DetectTargets_MinionBase_Test; // �� è�Ǿ� ������ �迭
	// �ӽ�---------------


	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> DetectTargets_EnemyChampion; // �� è�Ǿ� ������ �迭
	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> DetectTargets_SuperOrCanon; // ���� Ȥ�� ���� �̴Ͼ� ������ �迭
	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> DetectTargets_Warrior; // ���� �̴Ͼ� ������ �迭
	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> DetectTargets_Wizard; // ������ �̴Ͼ� ������ �迭

	// ������ ����� ������ ����
	UPROPERTY(VisibleAnywhere)
	class AActor* CurrentTarget; // ���� ���� Ÿ�� ����
	UPROPERTY(VisibleAnywhere)
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

public: // �������̽� �Լ�
	// �ǰݽ� ü���� ������ �� ȣ��
	virtual void Damaged() override;

	// �ǰݽ� ü���� 0 ������ �� ȣ��
	void Die() override;


};


