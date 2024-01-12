// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StateComponentBase.h"

#include <Net/UnrealNetwork.h>

#include "Interface/StateInterface.h"

// Sets default values for this component's properties
UStateComponentBase::UStateComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...

	Health = MaxHealth;	
}

// Called every frame
void UStateComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStateComponentBase, Health);
}

EFactionType UStateComponentBase::GetFactionType() const
{
	return FactionType;
}

void UStateComponentBase::SetFactionType(EFactionType NewFactionType)
{
	FactionType = NewFactionType;
}

void UStateComponentBase::ApplyDamage(float ADDamage, float APDamage)
{
	if (GetOwner()->HasAuthority() == false) return;
	
	float TotalDamage = ADDamage + APDamage;

	Health -= TotalDamage;
	OnRep_Health();
}

void UStateComponentBase::OnRep_Health()
{
	if (auto Interface = GetOwner<IStateInterface>())
	{
		if (Health > 0.f)
		{
			Interface->Damaged();
		}
		else
		{
			Interface->Die();
		}
	}
}