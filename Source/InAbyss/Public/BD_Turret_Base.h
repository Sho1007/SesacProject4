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


	// 포탑 주변의 챔피온 및 미니언을 감지할 Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* DetectCollisionComp;
	
	// 공격 시작 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* AttackStartPointComp;

public:
	// 감지기능

	// 감지된 대상들을 저장할 배열
	TArray <AActor*> DetectTargets_Test; // 나중에 필요없음

	TArray <AActor*> DetectTargets_EnemyChampion;
	TArray <AActor*> DetectTargets_SuperOrCanon;
	TArray <AActor*> DetectTargets_Warrior;
	TArray <AActor*> DetectTargets_Wizard;


	// 공격할 대상을 지정할 변수
	class AActor* CurrentTarget;
	class AActor* TopPriorityTarget; // 나중에 클래스를 챔피언으로 변경

public:
	// 공격대상 재지정 기능 
	void RetargetCurrentTarget();


public:
	// 공격기능
	void Attact_SpawnProjectile();

	// 일정 시간마다 대상을 공격

	float CurrentTIme;
	float TargetTIme = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectile_Turret> ProjectileFactory;



};
