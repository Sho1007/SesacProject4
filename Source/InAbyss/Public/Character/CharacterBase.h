// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/StateInterface.h"
#include "CharacterBase.generated.h"

UCLASS()
class INABYSS_API ACharacterBase : public ACharacter, public IStateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Damaged() override;
	
	virtual void Die() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Damaged_Blueprint();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Die_Blueprint();
};