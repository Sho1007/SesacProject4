// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathGrid.generated.h"

class USphereComponent;

UCLASS()
class INABYSS_API APathGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float DebugSphereRadius = 100.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	int32 DebugSphereSegment = 15; 
};