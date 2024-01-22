// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ezreal.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/StateComponentBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "InAbyss/InAbyss.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AEzreal::AEzreal()
{

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bDoCollisionTest = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));	
	CameraComponent->SetupAttachment(SpringArmComponent);

	StateComponent = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComponent"));
}

void AEzreal::BeginPlay()
{
	Super::BeginPlay();

	// if (HasAuthority() == true)
	// {
	// 	SetReplicates(true);
	// 	SetReplicateMovement(true);
	// 	// SetActorTickEnabled(true);
	// }

	
	// else
	// {
	// 	SetActorTickEnabled(false);
	// }

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

void AEzreal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEzreal, bIsMove);
	DOREPLIFETIME(AEzreal, ActorRotation);
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

	if (HasAuthority() == false) return;

	FVector MoveDirection =  Destination - GetActorLocation();
	MoveDirection.Z = 0.f;
	if (MoveDirection.Length() > ReachSuccessDistance)
	{
		bIsMove = true;
		MoveDirection.Normalize();

		AddActorWorldOffset(MoveDirection * MoveSpeed * DeltaSeconds);

		// AddMovementInput(MoveDirection, MoveSpeed * DeltaSeconds);
		
		ActorRotation = FRotationMatrix::MakeFromXZ(MoveDirection, GetActorUpVector()).Rotator();
		SetActorRotation(ActorRotation);
	}
	else
	{
		bIsMove = false;
	}
}



void AEzreal::Rotate()
{
}

void AEzreal::ServerRPC_RightClickStarted_Implementation(FVector WorldOrigin, FVector WorldDirection)
{
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * 100'000, ECC_Visibility))
	{
		if (UStateComponentBase* HitStateComponent = HitResult.GetActor()->GetComponentByClass<UStateComponentBase>())
		{
			if (HitStateComponent->GetFactionType() == StateComponent->GetFactionType())
			{
				Destination = HitResult.Location;
				// SetActorLocation(HitResult.Location);
			}
			else
			{
				// Todo Set Target;
			}
		}
		else
		{
			Destination = HitResult.Location;
			// SetActorLocation(HitResult.Location);
		}
		// PRINTLOG(TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetActorNameOrLabel());
	}
}

void AEzreal::RightClickStarted(const FInputActionValue& Value)
{
	APlayerController* PC = GetController<APlayerController>();
	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePosition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
		{
			FVector WorldOrigin;
			FVector WorldDirection;
			if (UGameplayStatics::DeprojectScreenToWorld(PC, MousePosition, WorldOrigin, WorldDirection) == true)
			{
				ServerRPC_RightClickStarted(WorldOrigin, WorldDirection);
				// FHitResult HitResult;
				// if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * 100'000, ECC_Visibility))
				// {
				// 	PRINTLOG(TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetActorNameOrLabel());
				// 	ServerRPC_RightClickStarted(WorldOrigin, WorldDirection);
				// }
			}
		}
	}
}

bool AEzreal::IsMove() const
{
	return bIsMove;
}

void AEzreal::OnRep_ActorRotation()
{
	SetActorRotation(ActorRotation);
}