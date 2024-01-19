// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFind/PathGrid.h"

#include "Components/SphereComponent.h"

// Sets default values
APathGrid::APathGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetSphereRadius(100.f);
}

// Called when the game starts or when spawned
void APathGrid::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APathGrid::OnComponentBeginOverlap);
}

// Called every frame
void APathGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// DrawDebugSphere(GetWorld(), GetActorLocation(), DebugSphereRadius, DebugSphereSegment, FColor::Cyan,
	// 	false, -1, 0, 3.f);
}

void APathGrid::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (OtherActor->GetComponentByClass<>());
}