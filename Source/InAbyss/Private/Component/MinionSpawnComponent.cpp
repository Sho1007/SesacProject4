// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MinionSpawnComponent.h"

#include "Component/StateComponentBase.h"
#include "Enemy/MinionBase.h"

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

		if (UStateComponentBase* StateComponentBase = GetOwner()->GetComponentByClass<UStateComponentBase>())
		{
			FactionType = StateComponentBase->GetFactionType();
		}
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

	CurrentMinionSpawnTime += DeltaTime;

	if (CurrentMinionSpawnTime >= MinionSpawnTime)
	{
		CurrentMinionSpawnTime -= MinionSpawnTime;

		SpawnMinion();
	}
}

void UMinionSpawnComponent::SpawnMinion()
{
	if (MeleeMinionClass)
	{
		FActorSpawnParameters Params;
		// Params.
		AMinionBase* Minion = GetWorld()->SpawnActor<AMinionBase>(MeleeMinionClass, SpawnLocation, FRotator(),
			Params);
		if (UStateComponentBase* MinionStateComponent = Minion->GetComponentByClass<UStateComponentBase>())
		{
			MinionStateComponent->SetFactionType(FactionType);
		}
	}
}

void UMinionSpawnComponent::SpawnMinionTimerFuntion()
{
}
