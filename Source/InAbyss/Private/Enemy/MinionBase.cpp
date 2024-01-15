// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MinionBase.h"

#include <Components/CapsuleComponent.h>
#include <Net/UnrealNetwork.h>
#include <Components/SphereComponent.h>
#include <Components/WidgetComponent.h>

#include "AnimInstance/MinionAnimInstance.h"
#include "Component/StateComponentBase.h"

#include "Widget/InGame/HealthBarWidgetBase.h"

// Sets default values
AMinionBase::AMinionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCollisionProfileName(TEXT("Object"));
	CapsuleComponent->SetCapsuleHalfHeight(30.f);
	CapsuleComponent->SetCapsuleRadius(20.f);
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetRelativeLocation(FVector(0, 0, -30.f));
	SkeletalMeshComponent->SetRelativeRotation(FRotator(0, -90.f,0));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(200.f);
	SphereComponent->SetCollisionProfileName(TEXT("BoidsSphere"));

	StateComponent = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComponent"));

	// HealthBarWidgetComponent
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(50.f, 5.f));
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts or when spawned
void AMinionBase::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicateMovement(true);

	if (HasAuthority() == true)
	{
		EnemyState = EEnemyState::IDLE;
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMinionBase::OnSphereComponentBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMinionBase::OnSphereComponentEndOverlap);
		SetActorTickEnabled(true);

		FindTarget();
	}
	else
	{
		SetActorTickEnabled(false);
	}

	AnimInstance = Cast<UMinionAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
	check(AnimInstance);
	if (UHealthBarWidgetBase* HealthBarWidget = Cast<UHealthBarWidgetBase>(HealthBarWidgetComponent->GetWidget()))
	{
		HealthBarWidget->InitStateComponent(StateComponent);
	}
}

// Called every frame
void AMinionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AnimInstance->IsAnyMontagePlaying()) return;

	if (Target != nullptr)
	{
		if (TargetStateComponent->IsDead())
		{
			Target = nullptr;
			return;
		}
		// Rotate To Target
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Z = 0.f;
		TargetDistance = Direction.Length();
		SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	}
	else
	{
		// Target is nullptr
		FindTarget();
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
				MultiRPC_PlayAttackMontage();
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

void AMinionBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMinionBase, EnemyState);
}

EEnemyState AMinionBase::GetEnemyState() const
{
	return EnemyState;
}

void AMinionBase::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || NeighborActorArray.Contains(OtherActor) || TargetActorArray.Contains(OtherActor)) return;

	if (UStateComponentBase* OverlapStateComponent = OtherActor->GetComponentByClass<UStateComponentBase>())
	{
		if (OverlapStateComponent->GetFactionType() == EFactionType::NEUTRAL) return;
		if (OverlapStateComponent->GetFactionType() == StateComponent->GetFactionType())
		{
			if (OverlapStateComponent->GetObjectType() == EObjectType::BUILDING) return;
			NeighborActorArray.Add(OtherActor);
		}
		else
		{
			if (OverlapStateComponent->GetObjectType() == EObjectType::BUILDING) return;
			TargetActorArray.Add(OtherActor);
		}
	}
}

void AMinionBase::OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TargetActorArray.Contains(OtherActor))
	{
		TargetActorArray.Remove(OtherActor);
	}
	else if (NeighborActorArray.Contains(OtherActor))
	{
		NeighborActorArray.Remove(OtherActor);
	}
	if (Target == OtherActor)
	{
		FindTarget();
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

void AMinionBase::Attack()
{
	if (HasAuthority() == false) return;

	UE_LOG(LogTemp,Warning, TEXT("AMinionBase::Attack) Has State Comp"));
	TargetStateComponent->ApplyDamage(StateComponent->GetAttackDamage(), StateComponent->GetAbilityPower());
}

void AMinionBase::SetTarget(AActor* NewTarget, int32 NewPriority)
{
	if (Target == nullptr)
	{
		Target = NewTarget;
		TargetStateComponent = Target->GetComponentByClass<UStateComponentBase>();
		if (NewPriority == -1)
		{
			TargetPriority = TargetStateComponent->GetObjectType() == EObjectType::CHAMPION ? 7 : 6;
		}
		else
		{
			TargetPriority = NewPriority;
		}
		TargetDistance = FVector::DistXY(GetActorLocation(), Target->GetActorLocation());
	}
	else
	{
		
		auto IterStateComponent =  NewTarget->GetComponentByClass<UStateComponentBase>();
		int IterPriority;
		if (NewPriority == -1)
		{
			IterPriority = IterStateComponent->GetObjectType() == EObjectType::CHAMPION ? 7 : 6; 
		}
		else
		{
			IterPriority = NewPriority;
		}
		if (IterPriority > TargetPriority) return;

		float IterDistance = FVector::DistXY(GetActorLocation(), NewTarget->GetActorLocation());
		if (IterDistance < TargetDistance)
		{
			Target = NewTarget;
			TargetStateComponent = IterStateComponent;
			TargetPriority = IterPriority;
			TargetDistance = IterDistance; 
		}
	}
}

void AMinionBase::FindTarget()
{
	TargetActorArray.Sort();
	Target = nullptr;
	if (TargetActorArray.Num() == 0)
	{
		return;
	}
	SetTarget(TargetActorArray[0]);
	for (auto Iter : TargetActorArray)
	{
		SetTarget(Iter);
	}

	// GetWorld()->GetGameState<AInGameGameState>();
}

void AMinionBase::MultiRPC_PlayAttackMontage_Implementation()
{
	// Todo : Play Attack Anim
	AnimInstance->PlayAttackMontage();
}
