// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MinionSpawnComponent.h"

#include "Component/StateComponentBase.h"
#include "Enemy/MinionBase.h"
#include "InAbyss/InAbyss.h"

// Sets default values for this component's properties
UMinionSpawnComponent::UMinionSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; 
	// ...
}


// Called when the game starts
void UMinionSpawnComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->HasAuthority() == true)
	{
		SetComponentTickEnabled(true);

		NexusStateComponent = GetOwner()->GetComponentByClass<UStateComponentBase>();
		if (NexusStateComponent)
		{
			FactionType = NexusStateComponent->GetFactionType();
		}

		PrepareMinion();
	}
	else
	{
		SetComponentTickEnabled(false);
	}
}


// Called every frame
void UMinionSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (NexusStateComponent && NexusStateComponent->IsDead())
	{
		SetComponentTickEnabled(false);
		return;
	}

	CurrentMinionSpawnTime += DeltaTime;

	if (CurrentMinionSpawnTime >= MinionSpawnTime)
	{
		CurrentMinionSpawnTime -= MinionSpawnTime;

		SpawnMinion();
	}
}

void UMinionSpawnComponent::PrepareMinion()
{
	// Params
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (MeleeMinionClass)
	{
		for (int i = 0; i < MeleeMinionPoolCount; ++i)
		{
			AMinionBase* Minion = GetWorld()->SpawnActor<AMinionBase>(MeleeMinionClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(),
			Params);
			Minion->SetWayPointArray(WayPointArray);
			Minion->Deactivate();
			if (UStateComponentBase* MinionStateComponent = Minion->GetComponentByClass<UStateComponentBase>())
			{
				MinionStateComponent->SetFactionType(FactionType);
			}

			MeleeMinionObjectPool.Add(Minion);
		}
	}

	if (CasterMinionClass)
	{
		for (int i = 0; i < CasterMinionPoolCount; ++i)
		{
			AMinionBase* Minion = GetWorld()->SpawnActor<AMinionBase>(CasterMinionClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(),
			Params);
			Minion->SetWayPointArray(WayPointArray);
			Minion->Deactivate();
			if (UStateComponentBase* MinionStateComponent = Minion->GetComponentByClass<UStateComponentBase>())
			{
				MinionStateComponent->SetFactionType(FactionType);
			}

			CasterMinionObjectPool.Add(Minion);
		}
	}

	if (SiegeMinionClass)
	{
		for (int i = 0; i < SiegeMinionPoolCount; ++i)
        {
        	AMinionBase* Minion = GetWorld()->SpawnActor<AMinionBase>(SiegeMinionClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(),
        	Params);
			Minion->SetWayPointArray(WayPointArray);
        	Minion->Deactivate();
        	if (UStateComponentBase* MinionStateComponent = Minion->GetComponentByClass<UStateComponentBase>())
        	{
        		MinionStateComponent->SetFactionType(FactionType);
        	}

        	SiegeMinionObjectPool.Add(Minion);
        }
	}
}

void UMinionSpawnComponent::SpawnMinion()
{
	// PRINTLOG(TEXT(""));
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(SpawnMinionTimerHandle);

	LineCount++;
	TimerManager.SetTimer(SpawnMinionTimerHandle, this, &UMinionSpawnComponent::SpawnMinionTimerFuntion, SpawnInterval, true);
}

void UMinionSpawnComponent::SpawnMinionTimerFuntion()
{
	if (SpawnIndex < 3)
	{
		if (LineCount % 3 == 0 && SpawnIndex == 0 && false /* bIsInhibitorDestroyed == true */ && bSuperMinionSpawned == false )
		{
			// Spawn Super Minion
			// bSuperMinionSpawned = true;
		}
		else
		{
			// Spawn Caster Minion
			for (auto Iter : MeleeMinionObjectPool)
			{
				if (Iter->IsActivated() == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("UMinionSpawnComponent::SpawnMinionTimerFuntion) Activated Object : %s"), *Iter->GetActorNameOrLabel());
					Iter->Activate();
					break;
				}
			}

			SpawnIndex++;
		}
	}
	else
	{
		if (LineCount % 3 == 0 && SpawnIndex == 3 && bSuperMinionSpawned == false)
		{
			bSuperMinionSpawned = true;

			// Spawn Seige Minion
			for (auto Iter : SiegeMinionObjectPool)
			{
				if (Iter->IsActivated() == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("UMinionSpawnComponent::SpawnMinionTimerFuntion) Activated Object : %s"), *Iter->GetActorNameOrLabel());
					Iter->Activate();
					break;
				}
			}
		}
		else
		{
			// Spawn Caster Minion
			for (auto Iter : CasterMinionObjectPool)
			{
				if (Iter->IsActivated() == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("UMinionSpawnComponent::SpawnMinionTimerFuntion) Activated Object : %s"), *Iter->GetActorNameOrLabel());
					Iter->Activate();
					break;
				}
			}

			SpawnIndex++;
		}
	}
	
	if (SpawnIndex == 6)
	{
		bSuperMinionSpawned = false;
		SpawnIndex = 0; 
		GetWorld()->GetTimerManager().ClearTimer(SpawnMinionTimerHandle);
	}
}