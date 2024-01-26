// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/EzrealQ.h"

#include "Components/SphereComponent.h"

// Sets default values
AEzrealQ::AEzrealQ()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEzrealQ::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEzrealQ::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}