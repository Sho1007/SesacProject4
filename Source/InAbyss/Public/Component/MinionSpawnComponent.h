// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MinionSpawnComponent.generated.h"


enum class EFactionType : uint8;
class AMinionBase;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INABYSS_API UMinionSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMinionSpawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void PrepareMinion();
	
	// SpawnMinion Timer
	void SpawnMinion();
	
	UFUNCTION()
	void SpawnMinionTimerFuntion();

protected:
	FTimerHandle SpawnMinionTimerHandle;
	UPROPERTY(EditDefaultsOnly)
	float SpawnInterval = 0.5f;

	int32 SpawnIndex = 0;
	UPROPERTY(VisibleInstanceOnly)
	int32 LineCount = 0;
	
	UPROPERTY(VisibleInstanceOnly)
	EFactionType FactionType;
	
	UPROPERTY(EditDefaultsOnly)
	float MinionSpawnTime = 5.0f;
	UPROPERTY(VisibleInstanceOnly)
	float CurrentMinionSpawnTime = 0.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMinionBase> MeleeMinionClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMinionBase> CasterMinionClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMinionBase> SiegeMinionClass;

	UPROPERTY(EditInstanceOnly, Meta = (MakeEditWidget))
	FVector SpawnLocation;

	// ObjectPool
	bool bSuperMinionSpawned = false;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	int32 MeleeMinionPoolCount = 100;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	int32 CasterMinionPoolCount = 100;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	int32 SiegeMinionPoolCount = 34;
	
	UPROPERTY(VisibleInstanceOnly)
	TArray<AMinionBase*> MeleeMinionObjectPool;
	UPROPERTY(VisibleInstanceOnly)
	TArray<AMinionBase*> CasterMinionObjectPool;
	UPROPERTY(VisibleInstanceOnly)
	TArray<AMinionBase*> SiegeMinionObjectPool;
};