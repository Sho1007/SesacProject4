// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyProjectile.h"

#include "Component/StateComponentBase.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	SetReplicateMovement(true);
	SetActorTickEnabled(false);
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target == nullptr || Target->IsValidLowLevel() == false)
	{
		Destroy();
		return;
	}
	
	FVector TargetDirection = Target->GetActorLocation() - GetActorLocation();
	TargetDirection.Z = 0.f;
	float TargetDistance = TargetDirection.Length();

	if (TargetDistance > HitDistance)
	{
		TargetDirection.Normalize();

		AddActorWorldOffset(TargetDirection * MoveSpeed * DeltaTime);
		return;
	}

	if (UStateComponentBase* TargetStateComponent = Target->GetComponentByClass<UStateComponentBase>())
	{
		TargetStateComponent->ApplyDamage(AttackDamage, AbilityPower);
	}
	
	Destroy();
}

void AEnemyProjectile::SetTarget(AActor* NewTarget)
{
	if (NewTarget == nullptr || NewTarget->IsValidLowLevel() == false) return;
	
	Target = NewTarget;

	SetActorTickEnabled(true);
}

void AEnemyProjectile::SetDamage(float NewAttackDamage, float NewAbilityPower)
{
	AttackDamage = NewAttackDamage;
	AbilityPower = NewAbilityPower;
}