// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

UCLASS()
class INABYSS_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTarget(AActor* NewTarget);
	void SetDamage(float NewAttackDamage = 0.f, float NewAbilityPower = 0.f); 

private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AActor* Target;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float HitDistance = 10.f;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float MoveSpeed = 300.f;

	float AttackDamage = 0.f;
	float AbilityPower = 0.f;
};