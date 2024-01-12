// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponentBase.generated.h"


UENUM()
enum class EFactionType
{
	NONE,
	RED,
	BLUE,
	NEUTRAL,
	SIZE,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	EFactionType GetFactionType() const;
	// Setter
	void SetFactionType(EFactionType NewFactionType);
	
	void ApplyDamage(float ADDamage = 0.f, float APDamage = 0.f);

	UFUNCTION()
	void OnRep_Health();
	
protected:
	UPROPERTY(VisibleAnywhere)
	EFactionType FactionType;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = "OnRep_Health")
	float Health;
};