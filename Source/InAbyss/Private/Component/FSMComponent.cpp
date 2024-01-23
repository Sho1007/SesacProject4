// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FSMComponent.h"

#include <EnhancedInputComponent.h>
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>
#include <Net/UnrealNetwork.h>

#include "Character/Ezreal.h" 
#include "Component/StateComponentBase.h"
#include "InAbyss/InAbyss.h"

// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UFSMComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFSMComponent, Destination);
}


// Called when the game starts
void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// Todo : Have to Change Owenr Type To ACharacterBase After.
	Owner = GetOwner<AEzreal>();

	// Find Other Component
	StateComponent = Owner->GetComponentByClass<UStateComponentBase>();

	if (Owner->IsLocallyControlled())
	{
		PlayerController = Owner->GetController<APlayerController>();
		LocalPlayer = PlayerController->GetLocalPlayer();
	}
}

// Called every frame
void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	
	// FString DebugText = TEXT("Owner : ") + Owner->GetActorNameOrLabel() + TEXT("\n");
	// DebugText += TEXT("LocalRole : ") + UEnum::GetValueAsString<ENetRole>(Owner->GetLocalRole()) + TEXT("\n");
	// DebugText += TEXT("RemoteRole : ") + UEnum::GetValueAsString<ENetRole>(Owner->GetRemoteRole()) + TEXT("\n");
	// DrawDebugString(GetWorld(), FVector(), DebugText, Owner, FColor::Yellow,
	// 	0.01);
	
	Rotate(DeltaTime);

	// PRINTLOG(TEXT("Character : %s"), *Owner->GetActorNameOrLabel());

	FVector MoveDirection =  Destination - Owner->GetActorLocation();
	MoveDirection.Z = 0.f;
	if (MoveDirection.Length() > ReachSuccessDistance)
	{
		bIsMove = true;
		MoveDirection.Normalize();

		Owner->AddMovementInput(MoveDirection, MoveSpeed * DeltaTime);
	}
	else
	{
		bIsMove = false;
	}

	if (Owner->IsLocallyControlled() == false) return;

	// GetMousePos
	if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
	{
		UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldOrigin, WorldDirection);
	}
}

void UFSMComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (GetOwner<ACharacter>()->IsLocallyControlled() == false) return;
	EnhancedInputComponent->BindAction(IA_RightClick, ETriggerEvent::Started, this, &UFSMComponent::RightClickStarted);
}

bool UFSMComponent::Rotate(float DeltaTime)
{
	CurrentRotationTime += DeltaTime;

	Owner->SetActorRotation(FMath::RInterpTo(FromRotation, ToRotation, CurrentRotationTime, RotationSpeed));
	
	return ToRotation == Owner->GetActorRotation();
}

void UFSMComponent::RightClickStarted(const FInputActionValue& Value)
{
	if (Owner->HasAuthority() == true)
	{
		ServerRPC_RightClickStarted_Implementation(WorldOrigin, WorldDirection);
	}
	else
	{
		ServerRPC_RightClickStarted(WorldOrigin, WorldDirection);
	}
}

void UFSMComponent::ServerRPC_RightClickStarted_Implementation(FVector NewWorldOrigin, FVector NewWorldDirection)
{
	// PRINTLOG(TEXT("WorldOrigin : %s, WorldDirection : %s"), *NewWorldOrigin.ToString(), *NewWorldDirection.ToString());
	
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, NewWorldOrigin,
	NewWorldOrigin + NewWorldDirection * 100'000, ECC_Visibility))
	{
		// PRINTLOG(TEXT("DebugPoint 1"));
		Destination = HitResult.Location;
		OnRep_Destination();
	}
}

bool UFSMComponent::IsMove() const
{
	return bIsMove;
}

void UFSMComponent::OnRep_Destination()
{
	if (Owner->IsLocallyControlled())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cursor, Destination);
	}
	Destination.Z = Owner->GetActorLocation().Z;
	// PRINTLOG(TEXT("Destinantion : %s"), *Destination.ToString());

	CurrentRotationTime = 0.f;
	
	FromRotation = Owner->GetActorRotation();
	ToRotation =  FRotationMatrix::MakeFromX((Destination - Owner->GetActorLocation()).GetSafeNormal()).Rotator();
}

// void UFSMComponent::ServerRPC_RightClickStarted_Implementation(FVector NewWorldOrigin, FVector NewWorldDirection)
// {
// 	PRINTLOG(TEXT(""));
// 	FHitResult HitResult;
// 	if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin,
// 		NewWorldOrigin + NewWorldDirection * 100'000, ECC_Visibility))
// 	{
// 		if (UStateComponentBase* HitStateComponent = HitResult.GetActor()->GetComponentByClass<UStateComponentBase>())
// 		{
// 			if (HitStateComponent->GetFactionType() != StateComponent->GetFactionType())
// 			{
// 				// Todo Set Target;
// 				return;
// 			}
// 		} 
//
// 		Destination = HitResult.Location;
// 		PRINTLOG(TEXT("Destinantion : %s"), *Destination.ToString());
// 		OnRep_Destination();
// 		// MultiRPC_SetDestination(HitResult.Location);
// 	}
// }
