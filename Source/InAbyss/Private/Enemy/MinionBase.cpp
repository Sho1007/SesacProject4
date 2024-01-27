// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MinionBase.h"

#include <Components/CapsuleComponent.h>
#include <Net/UnrealNetwork.h>
#include <Components/SphereComponent.h>
#include <Components/WidgetComponent.h>

#include "NetworkMessage.h"
#include "AnimInstance/MinionAnimInstance.h"
#include "Component/StateComponentBase.h"
#include "Enemy/EnemyProjectile.h"
#include "InAbyss/InAbyss.h"

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

	if (HasAuthority() == true)
	{
		EnemyState = EEnemyState::IDLE;
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMinionBase::OnSphereComponentBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMinionBase::OnSphereComponentEndOverlap);
		SetActorTickEnabled(true);
		SetReplicates(true);
		SetReplicateMovement(true);

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

	if (StateComponent->IsDead())
	{
		return;
	}

	switch (EnemyState) {
	case EEnemyState::MOVE:
		{
			if (WayPointIndex >= WayPointArray.Num()) return;

			FVector WayPointDirection = WayPointArray[WayPointIndex]->GetActorLocation() - GetActorLocation();
			WayPointDirection.Z = 0.f;
			// Todo : 멤버 변수화 필요
			float ReachSuccessDistance = 200.f;


			FVector NexusLocation = WayPointArray[WayPointArray.Num() - 1]->GetActorLocation();
			
			
			if (FVector::DistXY(NexusLocation, GetActorLocation()) > FVector::DistXY(NexusLocation, WayPointArray[WayPointIndex]->GetActorLocation()))
			{
				WayPointDirection.Normalize();
				if (NeighborActorArray.Num() > 0)
				{
					// DrawDebugSphere(GetWorld(), GetActorLocation(), SeparationDistance, 20, FColor::Yellow);
					FVector SeparationVector = Separation() * SeparationWeight;
				
					
					WayPointDirection += SeparationVector;
					WayPointDirection.Normalize();
				}
				SetActorRotation(FMath::Lerp(GetActorRotation(), FRotationMatrix::MakeFromX(WayPointDirection).Rotator(), DeltaTime * 5.f));
				
				AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime, true);
			}
			else
			{
				WayPointIndex++;
			}
		}
		
		break;
	case EEnemyState::ATTACK:
		if (Target == nullptr || TargetStateComponent->IsDead())
		{
			FindTarget();
			if (Target == nullptr) EnemyState = EEnemyState::MOVE;
		}
		else
		{
			FVector TargetDirection = Target->GetActorLocation() - GetActorLocation();
			TargetDirection.Z = 0.f;
			TargetDistance = TargetDirection.Length();
			if (TargetDistance <= AttackDistance)
			{
				if (bIsAttacking == false)
				{
					SetActorRotation(FRotationMatrix::MakeFromX(TargetDirection).Rotator());
					bIsAttacking = true;
					MultiRPC_PlayAttackMontage();
				}
			}
			else
			{
				TargetDirection.Normalize();
				if (NeighborActorArray.Num() > 0)
				{
					// DrawDebugSphere(GetWorld(), GetActorLocation(), SeparationDistance, 20, FColor::Yellow);
					FVector SeparationVector = Separation() * SeparationWeight;
				
					
					TargetDirection += SeparationVector;
					TargetDirection.Normalize();
				}
				SetActorRotation(FMath::Lerp(GetActorRotation(), FRotationMatrix::MakeFromX(TargetDirection).Rotator(), DeltaTime * 5.f));
				
				AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime, true);
			}
		}
		break;
	}

	

	
	
	
	/*
	Todo : 기존 FSM, WayPointTravel 구현 후 이것도 적용 필요
	if (bIsAttacking == true) return;

	FVector TargetVector;
	if (Target != nullptr)
	{
		if (TargetStateComponent->IsDead())
		{
			Target = nullptr;
			return;
		}
		TargetVector = Target->GetActorLocation() - GetActorLocation();
		TargetVector.Z = 0.f;
		TargetDistance = TargetVector.Length();
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
				TargetVector.Normalize();
				if (NeighborActorArray.Num() > 0)
				{
					// DrawDebugSphere(GetWorld(), GetActorLocation(), SeparationDistance, 20, FColor::Yellow);
					FVector SeparationVector = Separation() * SeparationWeight;
				
					
					TargetVector += SeparationVector;
					TargetVector.Normalize();
				}
				SetActorRotation(FMath::Lerp(GetActorRotation(), FRotationMatrix::MakeFromX(TargetVector).Rotator(), DeltaTime * 5.f));
				
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
		if (Target != nullptr && TargetStateComponent != nullptr)
		{
			if (TargetStateComponent->GetHPPercent() <= 0.f)
			{
				EnemyState = EEnemyState::IDLE;
				break;
			}
			
			if (TargetDistance > AttackDistance)
			{
				EnemyState = EEnemyState::MOVE;
			}
			else
			{
				if (bIsAttacking == false)
				{
					bIsAttacking = true;
					SetActorRotation(FRotationMatrix::MakeFromX(TargetVector).Rotator());
					MultiRPC_PlayAttackMontage();
				}
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

	*/
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

	// PRINTLOG(TEXT("Overlapped Actor : %s"), *OtherActor->GetActorNameOrLabel());
	
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
			TargetActorArray.Add(OtherActor);
			if (Target == nullptr)
			{
				SetTarget(OtherActor);
			}
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
}

FVector AMinionBase::Separation()
{
	FVector SeparationVector = FVector::ZeroVector;
	int32 Count = 0;
	for (auto Iter : NeighborActorArray)
	{
		if (Iter->GetComponentByClass<UStateComponentBase>()->IsDead()) continue;
		FVector Direction = GetActorLocation() - Iter->GetActorLocation();
		Direction.Z = 0.f;
		float Distance = Direction.Length();
		
		if (Distance > SeparationDistance) continue;
		Count++;
		Direction /= Distance;
		SeparationVector += Direction;
	}

	for (auto Iter : TargetActorArray)
	{
		if (Iter == Target || Iter->GetComponentByClass<UStateComponentBase>()->IsDead()) continue;
		FVector Direction = GetActorLocation() - Iter->GetActorLocation();
		Direction.Z = 0.f;
		float Distance = Direction.Length();
		
		if (Distance > SeparationDistance) continue;
		Count++;
		Direction /= Distance;
		SeparationVector += Direction;
	}

	if (Count > 0)
	{
		SeparationVector /= Count;
		// DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (SeparationVector * 500.f), FColor::Cyan, false, -1, 0, 3.f);
	}

	return SeparationVector;
}

FVector AMinionBase::Alignment()
{
	FVector AlignmentVector = GetActorForwardVector();
	
	if (NeighborActorArray.Num() > 0)
	{
		for (auto Iter : NeighborActorArray)
		{
			AlignmentVector += Iter->GetActorForwardVector();
		}
		
		AlignmentVector /= NeighborActorArray.Num();
		AlignmentVector.Normalize();
	}

	return AlignmentVector;
}

FVector AMinionBase::Cohesion()
{
	FVector CohesionVector = FVector::ZeroVector;

	if (NeighborActorArray.Num() > 0)
	{
		for (auto Iter : NeighborActorArray)
		{
			CohesionVector += Iter->GetActorLocation();
		}

		CohesionVector /= NeighborActorArray.Num();
		CohesionVector -= GetActorLocation();
		CohesionVector.Normalize();
	}

	return CohesionVector;
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
	MultiRPC_PlayDeathMontage();
}

void AMinionBase::Activate()
{
	EnemyState = EEnemyState::MOVE;
	bIsActivated = true;
	bIsAttacking = false;
	WayPointIndex = 0;
	Target = nullptr;
	StateComponent->SetHealthToMax();
	SetActorLocation(WayPointArray[0]->GetActorLocation());
	SetActorRotation(WayPointArray[0]->GetActorRotation());
	SetActorTickEnabled(true);
	MultiRPC_Activate();
}

void AMinionBase::Deactivate()
{
	PRINTLOG(TEXT("Begin"));
	bIsActivated = false;
	SetActorTickEnabled(false);
	MultiRPC_Deactivate();
}

bool AMinionBase::IsActivated()
{
	return bIsActivated;
}

void AMinionBase::Attack()
{
	if (HasAuthority() == false) return;
	
	// 근거리
	if (EnemyProjectileClass == nullptr)
	{
		TargetStateComponent->ApplyDamage(StateComponent->GetAttackDamage(), StateComponent->GetAbilityPower());
	}
	// 원거리
	else
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(TEXT("ProjectileSocket"));
		AEnemyProjectile* Projectile = GetWorld()->SpawnActor<AEnemyProjectile>(EnemyProjectileClass, SocketLocation, GetActorRotation(), SpawnParameters);
		Projectile->SetDamage(StateComponent->GetAttackDamage(), StateComponent->GetAbilityPower());
		Projectile->SetTarget(Target);
	}
}

void AMinionBase::EndAttack()
{
	bIsAttacking = false;
}

void AMinionBase::SetTarget(AActor* NewTarget, int32 NewPriority)
{
	if (NewTarget->GetComponentByClass<UStateComponentBase>()->IsDead()) return;
	
	EnemyState = EEnemyState::ATTACK;
	if (Target == nullptr)
	{
		Target = NewTarget;
		TargetStateComponent = Target->GetComponentByClass<UStateComponentBase>();
		if (NewPriority == -1)
		{
			switch(TargetStateComponent->GetObjectType())
			{
			case EObjectType::MINION:
				TargetPriority = 6;
				break;
			case EObjectType::SUPERMINION:
				TargetPriority = 6;
				break;
			case EObjectType::BUILDING:
				TargetPriority = 8;
				break;
			case EObjectType::CHAMPION:
				TargetPriority = 7;
				break;
			}
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
		int IterPriority = 9;
		if (NewPriority == -1)
		{
			switch(IterStateComponent->GetObjectType())
			{
			case EObjectType::MINION:
				IterPriority = 6;
				break;
			case EObjectType::SUPERMINION:
				IterPriority = 6;
				break;
			case EObjectType::BUILDING:
				IterPriority = 8;
				break;
			case EObjectType::CHAMPION:
				IterPriority = 7;
				break;
			}
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

void AMinionBase::SetWayPointArray(const TArray<AActor*>& NewWayPointArray)
{
	WayPointArray = NewWayPointArray;
}

void AMinionBase::MultiRPC_PlayDeathMontage_Implementation()
{
	HealthBarWidgetComponent->SetVisibility(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AnimInstance->PlayDeathMontage();
}

void AMinionBase::MultiRPC_Activate_Implementation()
{
	HealthBarWidgetComponent->SetVisibility(true);
	SkeletalMeshComponent->SetVisibility(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMinionBase::MultiRPC_Deactivate_Implementation()
{
	HealthBarWidgetComponent->SetVisibility(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetVisibility(false);
}

void AMinionBase::FindTarget()
{
	TargetActorArray.Sort();
	Target = nullptr;
	for (auto Iter : TargetActorArray)
	{
		SetTarget(Iter);
	}

	// GetWorld()->GetGameState<AInGameGameState>();
}

void AMinionBase::MultiRPC_PlayAttackMontage_Implementation()
{
	AnimInstance->PlayAttackMontage();
}
