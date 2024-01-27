// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_Turret.generated.h"

UCLASS()
class INABYSS_API AProjectile_Turret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Turret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraComponent* NSComp;



	// Owner값을 저장할 변수
	class ATurret_Base* OwnerTurret;

	// 타겟팅 대상을 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* AttackTarget;

	float Speed = 1000.f;


public:
	// Owner값을 저장하는 기능
	void OwnerSetting();

	// 공격 대상을 타겟팅하는 기능
	void AttackTargeting();

	// 발사체가 AttackTarget과 overlap된 경우 호출할 기능 
	void Attack_OverlapTarget();

	void SetAttackDamage(float NewAttackDamage, float NewAbilityDamage);


	float ADDamage = 10.f;
	float APDamage = 10.f;

};