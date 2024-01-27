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



	// Owner���� ������ ����
	class ATurret_Base* OwnerTurret;

	// Ÿ���� ����� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* AttackTarget;

	float Speed = 1000.f;


public:
	// Owner���� �����ϴ� ���
	void OwnerSetting();

	// ���� ����� Ÿ�����ϴ� ���
	void AttackTargeting();

	// �߻�ü�� AttackTarget�� overlap�� ��� ȣ���� ��� 
	void Attack_OverlapTarget();

	void SetAttackDamage(float NewAttackDamage, float NewAbilityDamage);


	float ADDamage = 10.f;
	float APDamage = 10.f;

};