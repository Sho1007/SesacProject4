// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building_Base.h"
#include "BD_Turret_Base.generated.h"

/**
 *
 */
UCLASS()
class INABYSS_API ABD_Turret_Base : public ABuilding_Base
{
	GENERATED_BODY()

public:
	ABD_Turret_Base();

	void Tick(float DeltaTime) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;


	// ��ž �ֺ��� è�ǿ� �� �̴Ͼ��� ������ Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* DetectCollisionComp;
	
	// ���� ���� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* AttackStartPointComp;

public:
	// �������

	// ������ ������ ������ �迭
	TArray <AActor*> DetectTargets_Test; // ���߿� �ʿ����

	TArray <AActor*> DetectTargets_EnemyChampion;
	TArray <AActor*> DetectTargets_SuperOrCanon;
	TArray <AActor*> DetectTargets_Warrior;
	TArray <AActor*> DetectTargets_Wizard;


	// ������ ����� ������ ����
	class AActor* CurrentTarget;
	class AActor* TopPriorityTarget; // ���߿� Ŭ������ è�Ǿ����� ����

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



};
