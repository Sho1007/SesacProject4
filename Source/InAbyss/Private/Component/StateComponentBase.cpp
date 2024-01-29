// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StateComponentBase.h"

#include <Net/UnrealNetwork.h>

#include "Character/CharacterBase.h"
#include "Interface/StateInterface.h"
#include "Widget/InGame/HealthBarWidgetBase.h"

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

	if (GetOwner()->HasAuthority())
	{
		SetIsReplicated(true);
		
		SetHealth(MaxHealth);
	}
}

// Called every frame
void UStateComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	FString DebugString = UEnum::GetValueAsString(FactionType);
	
	DrawDebugString(GetWorld(), FVector::ZeroVector, DebugString, GetOwner(),
		FColor::Cyan, 0.01f);

	// ...
}

void UStateComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStateComponentBase, Health);
	DOREPLIFETIME(UStateComponentBase, Mana);
	DOREPLIFETIME(UStateComponentBase, AttackDamage);
	DOREPLIFETIME(UStateComponentBase, AbilityPower);
	DOREPLIFETIME(UStateComponentBase, AttackRange);
	DOREPLIFETIME(UStateComponentBase, FactionType);
	DOREPLIFETIME(UStateComponentBase, ObjectType);
	DOREPLIFETIME(UStateComponentBase, bLuxBarrier);
	DOREPLIFETIME(UStateComponentBase, bSilence);
}

EFactionType UStateComponentBase::GetFactionType() const
{
	return FactionType;
}

EObjectType UStateComponentBase::GetObjectType() const
{
	return ObjectType;
}

float UStateComponentBase::GetHPPercent() const
{
	return Health / MaxHealth;
}

float UStateComponentBase::GetAbilityPower() const
{
	return AbilityPower;
}

float UStateComponentBase::GetAttackDamage() const
{
	return AttackDamage;
}

float UStateComponentBase::GetAttackRange() const
{
	return AttackRange;
}

bool UStateComponentBase::IsDead() const
{
	return Health <= 0.f;
}

void UStateComponentBase::SetFactionType(EFactionType NewFactionType)
{
	FactionType = NewFactionType;
}

void UStateComponentBase::SetObjectType(EObjectType NewObjectType)
{
	ObjectType = NewObjectType;
}

void UStateComponentBase::SetHealthToMax()
{
	SetHealth(MaxHealth);
}

void UStateComponentBase::SetAbilityPower(float NewAbilityPower)
{
	AbilityPower = NewAbilityPower;
}

void UStateComponentBase::SetAttackDamage(float NewAttackDamage)
{
	AttackDamage = NewAttackDamage;
}

void UStateComponentBase::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	OnRep_Health();
}

void UStateComponentBase::SetMana(float NewMana)
{
	Mana = FMath::Clamp(NewMana, 0.f, MaxMana);
	OnRep_Mana();
}

void UStateComponentBase::ApplyDamage(float ADDamage, float APDamage)
{
	if (GetOwner()->HasAuthority() == false) return;

	if (IsDead()) return;

	float TotalDamage = ADDamage + APDamage;
	
	if (bLuxBarrier)
	{
		TotalDamage *= 0.5f;
		bLuxBarrier = false;
		if (ACharacterBase* Character = GetOwner<ACharacterBase>())
		{
			Character->RemoveLuxBarrier();
		}
	}

	SetHealth(Health - TotalDamage);

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

void UStateComponentBase::AddHealth(float AddAmount)
{
	// Todo : 20240116 OSH 
	// Todo : If need to use Over Healed Health Amount, use this
	float OverHealthAmount = (AddAmount + Health) > MaxHealth ? (AddAmount + Health) - MaxHealth : 0.f;
	
	SetHealth(AddAmount + Health);
}

void UStateComponentBase::SetLuxBarrier(bool bNewLuxBarrier)
{
	if (GetOwner()->HasAuthority() == false) return;

	bLuxBarrier = bNewLuxBarrier;
}

void UStateComponentBase::SetSilence(bool bNewSilence)
{
	if (GetOwner()->HasAuthority() == false) return;
	bSilence = bNewSilence;

	if (SilenceTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SilenceTimerHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(SilenceTimerHandle, this, &UStateComponentBase::TurnOffSilence, 0.f, false, 3.f);
}

void UStateComponentBase::TurnOffSilence()
{
	bSilence = false;
}

void UStateComponentBase::AddExp(const float NewExp)
{
}

void UStateComponentBase::AddMoney(const int32 NewMoney)
{
}

void UStateComponentBase::OnRep_Health()
{
	OnHPChanged.Broadcast(Health / MaxHealth);
}

void UStateComponentBase::OnRep_Mana()
{
	OnMPChanged.Broadcast(Mana / MaxMana);
}

void UStateComponentBase::OnRep_AttackDamage()
{
}

void UStateComponentBase::OnRep_AbilityPower()
{
}

void UStateComponentBase::OnRep_AttackRange()
{
}
