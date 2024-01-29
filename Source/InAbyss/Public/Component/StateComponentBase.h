// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponentBase.generated.h"


UENUM()
enum class EFactionType : uint8
{
	NONE,
	RED,
	BLUE,
	NEUTRAL,
	SIZE,
};

UENUM()
enum class EObjectType : uint8
{
	NONE,
	MINION,
	SUPERMINION,
	BUILDING,
	CHAMPION,
	JUNGLE,
	SIZE,
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INABYSS_API UStateComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// Getter
	UFUNCTION(BlueprintCallable)
	EFactionType GetFactionType() const;
	UFUNCTION(BlueprintCallable)
	EObjectType GetObjectType() const;
	UFUNCTION(BlueprintCallable)
	float GetHPPercent() const;
	UFUNCTION(BlueprintCallable)
	float GetAbilityPower() const;
	UFUNCTION(BlueprintCallable)
	float GetAttackDamage() const;
	UFUNCTION(BlueprintCallable)
	float GetAttackRange() const;
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	
	// Setter
	void SetFactionType(EFactionType NewFactionType);
	void SetObjectType(EObjectType NewObjectType);

	void SetHealthToMax();

private:
	void SetAbilityPower(float NewAbilityPower);
	void SetAttackDamage(float NewAttackDamage);

	void SetHealth(float NewHealth);
	void SetMana(float NewMana);
public:
	// Health
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float ADDamage = 0.f, float APDamage = 0.f);
	UFUNCTION(BlueprintCallable)
	void AddHealth(float AddAmount);

	// LuxBarrier
	UFUNCTION(BlueprintCallable)
	void SetLuxBarrier(bool bNewLuxBarrier);

	void SetSilence(bool bNewSilence);
	UFUNCTION(BlueprintCallable)
	bool GetSilence() const;
	void TurnOffSilence();
	
	// Exp
	void AddExp(const float NewExp);

	// Money
	void AddMoney(const int32 NewMoney);

	// Mana

	UFUNCTION()
	void OnRep_Health();
	UFUNCTION()
	void OnRep_Mana();
	UFUNCTION()
	void OnRep_AttackDamage();
	UFUNCTION()
	void OnRep_AbilityPower();
	UFUNCTION()
	void OnRep_AttackRange();
protected:
	UPROPERTY(EditAnywhere)
	float Exp;
	UPROPERTY(EditAnywhere)
	int32 Money;
	
	UPROPERTY(Replicated, EditAnywhere)
	EFactionType FactionType;
	UPROPERTY(Replicated, EditAnywhere)
	EObjectType ObjectType;

	// 
	UPROPERTY(Replicated, EditAnywhere)
	bool bLuxBarrier = false;
	UPROPERTY(Replicated, EditAnywhere)
	bool bSilence = false;

	FTimerHandle SilenceTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = "OnRep_Health")
	float Health;

	UPROPERTY(EditDefaultsOnly)
	float MaxMana = 100.f;
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = "OnRep_Mana")
	float Mana;

	UPROPERTY(EditAnywhere, ReplicatedUsing = "OnRep_AttackDamage")
	float AttackDamage;
	UPROPERTY(EditAnywhere, ReplicatedUsing = "OnRep_AbilityPower")
	float AbilityPower;

	UPROPERTY(EditAnywhere, ReplicatedUsing = "OnRep_AttackRange")
	float AttackRange = 150.f;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FDELE_MULTI_ONE_FLOAT, float);
	FDELE_MULTI_ONE_FLOAT OnHPChanged;
	FDELE_MULTI_ONE_FLOAT OnMPChanged;
};

inline bool UStateComponentBase::GetSilence() const
{
	return bSilence;
}