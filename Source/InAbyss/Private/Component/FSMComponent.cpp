// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FSMComponent.h"

#include <EnhancedInputComponent.h>
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>
#include <Net/UnrealNetwork.h>

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
	DOREPLIFETIME(UFSMComponent, bIsMove);
}


// Called when the game starts
void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// Todo : Have to Change Owenr Type To ACharacterBase After.
	Owner = GetOwner<ACharacter>();

	// Find Other Component
	StateComponent = Owner->GetComponentByClass<UStateComponentBase>();

	if (Owner->IsLocallyControlled())
	{
		PlayerController = Owner->GetController<APlayerController>();
		if (PlayerController)
		{
			LocalPlayer = PlayerController->GetLocalPlayer();
		}
	}
}

void UFSMComponent::FindEnemy()
{
	float SearchRadius = 200.f;
	TArray<FOverlapResult> OutOverlaps;
	FCollisionQueryParams Params;

	DrawDebugSphere(GetWorld(), Owner->GetActorLocation(), SearchRadius, 20, FColor::Black,
		false, 5.0f, 0, 3);
	
	if (GetWorld()->OverlapMultiByProfile(OutOverlaps, Owner->GetActorLocation(), Owner->GetActorRotation().Quaternion(), FName("Object"),
		FCollisionShape::MakeSphere(SearchRadius), Params))
	{
		for (auto Iter : OutOverlaps)
		{
			PRINTLOG(TEXT("Overlapped Actor %s"), *Iter.GetActor()->GetActorNameOrLabel()); 
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
	
	switch (ChampionState)
	{
	case EChampionState::IDLE:
		// Todo : 가만히 있을 때 적이 다가오면, 공격하나? 하면 SphereComponent 써야할듯
		break;
	case EChampionState::ROTATE:
		{
			if (Rotate(DeltaTime))
			{
				CurrentRotationTime = 0.f;


				if (Owner->HasAuthority() == false) return;
				
				if (Target == nullptr)
				{
					ChampionState = EChampionState::MOVE;
				}
				else
				{
					ChampionState = EChampionState::ATTACK;
				}
			}
		}
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
				if (Owner->HasAuthority() == false) return;
				bIsMove = true;
			}
			else
			{
				if (Owner->HasAuthority() == false) return;
				bIsMove = false;
				ChampionState = EChampionState::IDLE;
				FindEnemy();
			}
		}
		break;
	case EChampionState::ATTACK:
		
		/* Todo 
		if (Target && TargetComponent->IsDead())
		if (bIsAttacking == false)
		{
		}
		*/
		break;
	case EChampionState::SKILL:
		break;
	}
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

void UFSMComponent::RightClickStarted(const FInputActionValue& Value)
{
	if (Owner->HasAuthority() == true)
	{
	}
	else
	{
	}
}

void UFSMComponent::ServerRPC_RightClickStarted_Implementation(FVector NewWorldOrigin, FVector NewWorldDirection)
{
}

void UFSMComponent::SetShouldStop(bool bNewShouldStop)
{
	bShouldStop = bNewShouldStop;
}

bool UFSMComponent::IsMove() const
{
	return bIsMove;
}

void UFSMComponent::OnRep_Destination()
{
	ACharacter* CurrentOwner = Cast<ACharacter>(GetOwner());

	if (CurrentOwner->IsLocallyControlled())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cursor, Destination);
	}

	Target = nullptr;
	
	Destination.Z = CurrentOwner->GetActorLocation().Z;

	CurrentRotationTime = 0.f;
	FromRotation = CurrentOwner->GetActorRotation();
	ToRotation =  FRotationMatrix::MakeFromX((Destination - CurrentOwner->GetActorLocation()).GetSafeNormal()).Rotator();
	ChampionState = EChampionState::MOVE;
}