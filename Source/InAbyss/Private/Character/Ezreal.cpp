// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ezreal.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AEzreal::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Inpu
	if (IsLocallyControlled() && DefaultIMC)
	{
		APlayerController* PC = GetController<APlayerController>();

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultIMC, 0);
		}
	}
}

void AEzreal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_RightClick, ETriggerEvent::Started, this, &AEzreal::RightClickStarted);
	}
}

void AEzreal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MoveDirection = GetVelocity().GetSafeNormal();
	
	FRotator NewRotator = FRotationMatrix::MakeFromXZ(MoveDirection, GetActorUpVector()).Rotator();
}

void AEzreal::Rotate()
{
}

void AEzreal::RightClickStarted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("AEzreal::RightClickStarted"));
}
