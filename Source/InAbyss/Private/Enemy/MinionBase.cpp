// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MinionBase.h"

#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>

#include "AnimInstance/MinionAnimInstance.h"
#include "Component/StateComponentBase.h"
#include "Components/SphereComponent.h"

// Sets default values
AMinionBase::AMinionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCollisionProfileName(TEXT("Minion"));
	CapsuleComponent->SetCapsuleHalfHeight(30.f);
	CapsuleComponent->SetCapsuleRadius(20.f);
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetRelativeLocation(FVector(0, 0, -30.f));
	SkeletalMeshComponent->SetRelativeRotation(FRotator(0, -90.f,0));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(200.f);
	SphereComponent->SetCollisionProfileName(TEXT("BoidsSphere"));

	StateComponent = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComponent"));
}

// Called when the game starts or when spawned
void AMinionBase::BeginPlay()
{
	Super::BeginPlay();

	Target = GetWorld()->GetFirstPlayerController()->GetCharacter();
	EnemyState = EEnemyState::IDLE;

	AnimInstance = Cast<UMinionAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
	check(AnimInstance);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMinionBase::OnSphereComponentBeginOverlap);
}

// Called every frame
void AMinionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AnimInstance->IsAnyMontagePlaying()) return;

	if (Target != nullptr)
	{
		// Rotate To Target
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Z = 0.f;
		TargetDistance = Direction.Length();
		SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	}

	switch (EnemyState) {
	case EEnemyState::IDLE:
		if (Target != nullptr)
		{
			if (TargetDistance > AttackDistance)
			{
				EnemyState = EEnemyState::MOVE;
			}
			else
			{
				EnemyState = EEnemyState::ATTACK;
			}
		}
		break;
	case EEnemyState::MOVE:
		if (Target != nullptr)
		{
			if (TargetDistance > AttackDistance)
			{
				AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime, true);
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
			if (TargetDistance > AttackDistance)
			{
				EnemyState = EEnemyState::MOVE;
			}
			else
			{
				// Todo Play AttackAnim;
				AnimInstance->PlayAttackMontage();
			}
		}
		else
		{
			EnemyState = EEnemyState::IDLE;
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

void AMinionBase::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("AMinionBase::OnCapsuleComponentBeginOverlap) Overlapped Actor : %s"), *OtherActor->GetActorNameOrLabel());
	if (Cast<AMinionBase>(OtherActor) && NeighborActorArray.Contains(OtherActor) == false)
	{
		NeighborActorArray.Add(OtherActor);
	}
}

void AMinionBase::Separation()
{
}

void AMinionBase::Alignment()
{
}

void AMinionBase::Cohesion()
{
	FVector Average;
	for (AActor* Iter : NeighborActorArray)
	{
		Average += Iter->GetActorLocation();
	}

	Average /= NeighborActorArray.Num();

	FVector CohesionVector = Average - GetActorLocation();

	// FMath::Lerp(GetActorForwardVector(), CohesionVector, 0.5f);
}

void AMinionBase::TestDamageFunction()
{
	StateComponent->ApplyDamage(10.f);
}

void AMinionBase::Damaged()
{
	
}

void AMinionBase::Die()
{
	AnimInstance->PlayDeathMontage();
	// Todo : Object Pool
}