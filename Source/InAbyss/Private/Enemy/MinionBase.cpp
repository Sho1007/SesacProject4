// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MinionBase.h"

#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>

#include "Component/StateComponentBase.h"

// Sets default values
AMinionBase::AMinionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	StateComponent = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComponent"));
}

// Called when the game starts or when spawned
void AMinionBase::BeginPlay()
{
	Super::BeginPlay();

	Target = GetWorld()->GetFirstPlayerController()->GetCharacter();
	EnemyState = EEnemyState::IDLE;
}

// Called every frame
void AMinionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (EnemyState) {
	case EEnemyState::IDLE:
		if (Target != nullptr)
		{
			float Distance = (Target->GetActorLocation() - GetActorLocation()).Length();
			if (Distance > AttackDistance)
			{
				EnemyState = EEnemyState::MOVE;
			}
		}
		break;
	case EEnemyState::MOVE:
		if (Target != nullptr)
		{
			// Rotate To Target
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			float Distance = Direction.Length();
			SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
			if (Distance > AttackDistance)
			{
				AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime);
			}
			else
			{
				EnemyState = EEnemyState::ATTACK;
			}
		}
		else
		{
			EnemyState = EEnemyState::IDLE; 
		}
		break;
	case EEnemyState::ATTACK:
		if (Target != nullptr)
		{
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			float Distance = Direction.Length();
			if (Distance > AttackDistance)
			{
				EnemyState = EEnemyState::MOVE;
			}
			else
			{
				// Todo Play AttackAnim;
			}
		}
		break;
	case EEnemyState::RETURN:
		break;
	}
}

EEnemyState AMinionBase::GetEnemyState() const
{
	return EnemyState;
}