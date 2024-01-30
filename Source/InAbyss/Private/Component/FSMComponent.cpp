// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FSMComponent.h"

#include <EnhancedInputComponent.h>
#include <NiagaraFunctionLibrary.h>
#include <Net/UnrealNetwork.h>
#include "Component/SkillComponent.h"
#include "AnimInstance/ChampionAnimInstance.h"
#include "Character/Ezreal.h" 
#include "Component/StateComponentBase.h"
#include "InAbyss/InAbyss.h"

// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSMComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFSMComponent, Destination);
	DOREPLIFETIME(UFSMComponent, SkillPoint);
	DOREPLIFETIME(UFSMComponent, Target);
	DOREPLIFETIME(UFSMComponent, bIsAttacking);
}


// Called when the game starts
void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// Todo : Have to Change Owenr Type To ACharacterBase After.
	Owner = GetOwner<ACharacter>();
	AnimInstance = Cast<UChampionAnimInstance>(Owner->GetMesh()->GetAnimInstance());

	if (Owner->HasAuthority() == true)
	{
		SetIsReplicated(true);
	}

	// Find Other Component
	StateComponent = Owner->GetComponentByClass<UStateComponentBase>();
	SkillComponent = Owner->GetComponentByClass<USkillComponent>();

	if (Owner->IsLocallyControlled())
	{
		PlayerController = Owner->GetController<APlayerController>();
	}
}

void UFSMComponent::FindEnemy()
{
	if (Owner->HasAuthority() == false) return;
	
	TArray<FOverlapResult> OutOverlaps;
	FCollisionQueryParams Params;

	DrawDebugSphere(GetWorld(), Owner->GetActorLocation(), StateComponent->GetAttackRange(), 20, FColor::Black,
		false, 1.0f, 0, 1);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(1.f);
	if (GetWorld()->OverlapMultiByProfile(OutOverlaps, Owner->GetActorLocation(), Owner->GetActorRotation().Quaternion(), FName("Object"),
		CollisionShape, Params))
	{
		for (auto Iter : OutOverlaps)
		{
			if (FVector::DistXY(Iter.GetActor()->GetActorLocation(), Owner->GetActorLocation()) > StateComponent->GetAttackRange()) continue;
			UStateComponentBase* OverlapStateComponent =  Iter.GetActor()->GetComponentByClass<UStateComponentBase>();
			if (OverlapStateComponent == nullptr) continue;

			if (OverlapStateComponent->GetFactionType() == EFactionType::NEUTRAL || OverlapStateComponent->GetFactionType() == StateComponent->GetFactionType()) continue;

			ServerRPC_SetTarget(Iter.GetActor());
			DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Iter.GetActor()->GetActorLocation(), FColor::Cyan, false, 1.f);
			break;
		}
	}
}

// Called every frame
void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerController)
	{
		FHitResult HitResult;

		bIsCursored = PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, HitResult);
		if (bIsCursored)
		{
			if (UStateComponentBase* HitStateComponent =  HitResult.GetActor()->GetComponentByClass<UStateComponentBase>())
			{
				if (HitStateComponent->GetFactionType() == StateComponent->GetFactionType())
				{
					CursoredTarget = nullptr;
					CursoredLocation = HitResult.Location;
				}
				else
				{
					CursoredTarget = HitResult.GetActor();
				}
			}
			else
			{
				CursoredTarget = nullptr;
				CursoredLocation = HitResult.Location;
			}
		}
	}

	// ...
	
	FString DebugString;
    	DebugString += "\n" + UEnum::GetValueAsString(ChampionState);
	
	switch (ChampionState)
	{
	case EChampionState::IDLE:
		// Todo : 가만히 있을 때 적이 다가오면, 공격하나? 하면 SphereComponent 써야할듯
		break;
	case EChampionState::MOVE:
		{
			Rotate(DeltaTime);
			FVector MoveDirection =  Destination - Owner->GetActorLocation();
			MoveDirection.Z = 0.f;
			if (MoveDirection.Length() > ReachSuccessDistance)
			{
				MoveDirection.Normalize();
				Owner->AddMovementInput(MoveDirection, MoveSpeed * DeltaTime);
			}
			else
			{
				ChampionState = EChampionState::IDLE;
				FindEnemy();
			}
		}
		break;
	case EChampionState::ATTACK:
		{
			// PRINTLOG(TEXT("ATTACK"));
			if (TargetStateComponent->IsDead())
			{
				PRINTLOG(TEXT("IsDead"));
				FindEnemy();
				break;
			}
			if (bIsAttacking == true) break;
			if (Target == nullptr || TargetStateComponent == nullptr) break;
			
			FVector TargetDirection = Target->GetActorLocation() - Owner->GetActorLocation();
			TargetDirection.Z = 0.f;

			if (TargetDirection.Length() > StateComponent->GetAttackRange())
			{
				if (Owner->HasAuthority())
				{
					EndAttack();
					MultiRPC_StopMontage();
				}

				DebugString += "1\n";
				
				RotateToTarget(DeltaTime);
				TargetDirection.Normalize();
				Owner->AddMovementInput(TargetDirection, MoveSpeed * DeltaTime);
			}
			else
			{
				if (RotateToTarget(DeltaTime) && Owner->IsLocallyControlled())
				{
					DebugString += "2\n";
					ServerRPC_PlayAttackAnim();
				}
				// 
				// PRINTLOG(TEXT("1"));
				// if (RotateToTarget(DeltaTime))
				// {
				// 	PRINTLOG(TEXT("2"));
				// 	PlayAttackAnim();
				// }
			}
		}
		break;

	case EChampionState::SKILL:
		{
			if (SkillComponent->IsSkilling()) return;
			if (RotateToSkillPoint(DeltaTime) && Owner->IsLocallyControlled())
			{
				SkillComponent->Q();
			}
		}
		break;
	}

	
	DrawDebugString(GetWorld(), FVector::ZeroVector, DebugString,
		Owner, FColor::Yellow, 0.01f);
}

void UFSMComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (GetOwner<ACharacter>()->IsLocallyControlled() == false) return;
	EnhancedInputComponent->BindAction(IA_RightClick, ETriggerEvent::Started, this, &UFSMComponent::RightClickStarted);
}

bool UFSMComponent::Rotate(float DeltaTime)
{
	CurrentRotationTime += DeltaTime;

	Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), ToRotation, CurrentRotationTime, RotationSpeed));
	
	return ToRotation.Equals(Owner->GetActorRotation());
}

bool UFSMComponent::RotateToTarget(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("UFSMComponent::RotateToTarget")));
	FVector TargetDirection = Target->GetActorLocation() - Owner->GetActorLocation();
	TargetDirection.Z = 0.f;
	ToRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();

	return Rotate(DeltaTime);
}

bool UFSMComponent::RotateToSkillPoint(float DeltaTime)
{
	CurrentRotationTime += DeltaTime;

	Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), SkillRotation, CurrentRotationTime, RotationSpeed));
	
	return SkillRotation.Equals(Owner->GetActorRotation());
}

void UFSMComponent::RightClickStarted(const FInputActionValue& Value)
{
	if (CursoredTarget != nullptr)
	{
		ServerRPC_SetTarget(CursoredTarget);
		Target = CursoredTarget;
		OnRep_Target();
	}
	else
	{
		ServerRPC_SetDestination(CursoredLocation);
		Destination = CursoredLocation;
		Target = nullptr;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cursor, Destination);
		OnRep_Destination();
	}
}

void UFSMComponent::ServerRPC_SetDestination_Implementation(FVector NewDestination)
{
	Destination = NewDestination;
	Target = nullptr;
	OnRep_Destination();
	bIsAttacking = false;
	if (SkillComponent->IsSkilling() == false)
	{
		MultiRPC_StopMontage();
	}
	// Todo : Skill 시 캔슬
}

void UFSMComponent::ServerRPC_SetTarget_Implementation(AActor* NewTarget)
{
	Target = NewTarget;
	OnRep_Target(); 
}

void UFSMComponent::MultiRPC_PlayAttackAnim_Implementation()
{
	AnimInstance->PlayAttackMontage();
}

void UFSMComponent::ServerRPC_PlayAttackAnim_Implementation()
{
	bIsAttacking = true;
	OnRep_IsAttacking();
	MultiRPC_PlayAttackAnim();
}

void UFSMComponent::MultiRPC_StopMontage_Implementation()
{
	AnimInstance->StopAllMontages(0.1f);
}

void UFSMComponent::ServerRPC_ApplyDamage_Implementation()
{
	TargetStateComponent->ApplyDamage(StateComponent->GetAttackDamage(), StateComponent->GetAbilityPower());
}

bool UFSMComponent::IsMove() const
{
	return ChampionState == EChampionState::MOVE || (Target && FVector::DistXY(Target->GetActorLocation(), Owner->GetActorLocation()) > StateComponent->GetAttackRange());
}

void UFSMComponent::EndAttack()
{
	if (Owner->HasAuthority() == false) return;
	
	bIsAttacking = false;
	OnRep_IsAttacking();
}

void UFSMComponent::EndSkill()
{
	MultiRPC_EndSKill();
}

void UFSMComponent::MultiRPC_EndSKill_Implementation()
{
	ChampionState = BeforeState;
}

void UFSMComponent::PrepareSkill()
{
	ServerRPC_SetSkillPoint(CursoredLocation);
	SkillPoint = CursoredLocation; 
	OnRep_SkillPoint();
}

void UFSMComponent::ServerRPC_SetSkillPoint_Implementation(FVector NewSkillPoint)
{
	SkillPoint = NewSkillPoint;
	OnRep_SkillPoint();
}

EChampionState UFSMComponent::GetChampionState() const
{
	return ChampionState;
}

void UFSMComponent::OnRep_Destination()
{
	ACharacter* CurrentOwner = Cast<ACharacter>(GetOwner());
	
	Destination.Z = CurrentOwner->GetActorLocation().Z;
	CurrentRotationTime = 0.f;
	FromRotation = CurrentOwner->GetActorRotation();
	ToRotation =  FRotationMatrix::MakeFromX((Destination - CurrentOwner->GetActorLocation()).GetSafeNormal()).Rotator();
	if (SkillComponent->IsSkilling() == false)
	{
		ChampionState = EChampionState::MOVE;
	}
	else
	{
		BeforeState = EChampionState::MOVE;
	}
	
}

void UFSMComponent::OnRep_SkillPoint()
{
	ACharacter* CurrentOwner = Cast<ACharacter>(GetOwner());
	
	SkillPoint.Z = CurrentOwner->GetActorLocation().Z;
	CurrentRotationTime = 0.f;
	SkillRotation =  FRotationMatrix::MakeFromX((SkillPoint - CurrentOwner->GetActorLocation()).GetSafeNormal()).Rotator();
	if (ChampionState != EChampionState::SKILL)
	{
		BeforeState = ChampionState;
	}
	ChampionState = EChampionState::SKILL;
}

void UFSMComponent::OnRep_Target()
{
	if (Target == nullptr) return;
	PRINTLOG(TEXT("Target : %s, Location : %s"), *Target->GetActorNameOrLabel(), *Target->GetActorLocation().ToString());
	CurrentRotationTime = 0.f;	
	ACharacter* CurrentOwner = Cast<ACharacter>(GetOwner());
	TargetStateComponent = Target->GetComponentByClass<UStateComponentBase>();

	if (SkillComponent->IsSkilling() == false)
	{
		ChampionState = EChampionState::ATTACK;
	}
	else
	{
		BeforeState = EChampionState::ATTACK;
	}
}

void UFSMComponent::OnRep_IsAttacking()
{
	PRINTLOG(TEXT("Value : %s"), bIsAttacking ? TEXT("True") : TEXT("False"));
}