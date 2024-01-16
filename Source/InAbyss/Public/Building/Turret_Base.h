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
	// 포탑 주변의 챔피온 및 미니언을 감지할 Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* DetectCollisionComp;
	
	// 공격 시작 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* AttackStartPointComp;

public: // 감지기능

	// 감지된 대상들을 저장할 배열
	// 임시---------------
	//TArray <AActor*> DetectTargets_Test; // 나중에 필요없음

	//TArray <AActor*> DetectTargets_Chracter_Test; // 적 챔피언 저장할 배열
	//TArray <AActor*> DetectTargets_MinionBase_Test; // 적 챔피언 저장할 배열
	// 임시---------------


	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> DetectTargets_EnemyChampion; // 적 챔피언 저장할 배열
	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> DetectTargets_SuperOrCanon; // 슈퍼 혹은 공성 미니언 저장할 배열
	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> DetectTargets_Warrior; // 전사 미니언 저장할 배열
	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> DetectTargets_Wizard; // 마법사 미니언 저장할 배열

	// 공격할 대상을 지정할 변수
	UPROPERTY(VisibleAnywhere)
	class AActor* CurrentTarget; // 공격 받을 타겟 변수
	UPROPERTY(VisibleAnywhere)
	class AActor* TopPriorityTarget; // CurrentTarget보다 먼저 공격받을 타겟 변수 // 나중에 클래스를 챔피언으로 변경

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

public: // 인터페이스 함수
	// 피격시 체력이 남았을 때 호출
	virtual void Damaged() override;

	// 피격시 체력이 0 이하일 때 호출
	void Die() override;


};


