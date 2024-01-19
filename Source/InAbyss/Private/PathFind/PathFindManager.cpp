// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFind/PathFindManager.h"

#include "PathFind/PathGrid.h"

// Sets default values
APathFindManager::APathFindManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathFindManager::BeginPlay()
{
	Super::BeginPlay();

	float StartX = FMath::Min(Edge1.X + GetActorLocation().X, Edge2.X + GetActorLocation().X);
	float StartY = FMath::Min(Edge1.Y + GetActorLocation().Y, Edge2.Y + GetActorLocation().Y);
	float EndX = FMath::Max(Edge1.X + GetActorLocation().X, Edge2.X + GetActorLocation().X); 
	float EndY = FMath::Max(Edge1.Y + GetActorLocation().Y, Edge2.Y + GetActorLocation().Y);

	UE_LOG(LogTemp, Warning, TEXT("APathFindManager::BeginPlay) Start : %f, %f, End : %f, %f"), StartX, StartY, EndX, EndY);
	UWorld* World = GetWorld();
	if (World == nullptr || PathGridClass == nullptr) return;
	FHitResult OutHit;
	FCollisionQueryParams Params;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (; StartX <= EndX; StartX += GridIntervalDistance)
	{
		// UE_LOG(LogTemp, Warning, TEXT("APathFindManager::BeginPlay) Pos : %f, %f"), StartX, StartY);
		// UE_LOG(LogTemp, Warning, TEXT("APathFindManager::BeginPlay) Pos : %f, %f"), StartX, StartY);
		StartY = FMath::Min(Edge1.Y + GetActorLocation().Y, Edge2.Y + GetActorLocation().Y);
		for (; StartY <= EndY; StartY += GridIntervalDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("APathFindManager::BeginPlay) Pos : %f, %f"), StartX, StartY);
			
			if (World->LineTraceSingleByChannel(OutHit, FVector(StartX, StartY, 5000.f), FVector(StartX, StartY, -5000.f), ECC_WorldStatic))
			{
				World->SpawnActor<APathGrid>(PathGridClass, OutHit.Location, FRotator(), SpawnParams);
				//DrawDebugLine(World, FVector(StartX, StartY, 5000.f), FVector(StartX, StartY, -5000.f), FColor::Cyan, true);
				// DrawDebugSphere(World, OutHit.Location, GridIntervalDistance / 2.f, 15, FColor::Cyan, true);
			}
		}
	UE_LOG(LogTemp, Warning, TEXT("APathFindManager::BeginPlay) Start : %f, %f, End : %f, %f"), StartX, StartY, EndX, EndY);
	}

}

// Called every frame
void APathFindManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}