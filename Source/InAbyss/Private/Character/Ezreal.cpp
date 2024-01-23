// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ezreal.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AnimInstance/EzrealAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Component/FSMComponent.h"
#include "Component/StateComponentBase.h"
#include "GameFramework/SpringArmComponent.h"
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
	StateComponent->SetIsReplicated(true);
	
	FSMComponent = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComponent"));
	FSMComponent->SetIsReplicated(true);
}

void AEzreal::BeginPlay()
{
	Super::BeginPlay();

	// AnimInstance Init
	AnimInstance = Cast<UEzrealAnimInstance>(GetMesh()->GetAnimInstance());

	// Enhanced Input
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
}

void AEzreal::MultiRPC_Q_Implementation()
{
	AnimInstance->SetQReady();
}

void AEzreal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsLocallyControlled() == false) return;

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Q, ETriggerEvent::Started, this, &AEzreal::QStarted);

		FSMComponent->SetupPlayerInputComponent(EnhancedInputComponent);
	}
}

void AEzreal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AEzreal::ServerRPC_Q_Implementation()
{
	MultiRPC_Q();
}

void AEzreal::QStarted(const FInputActionValue& Value)
{
	// Destination = GetActorLocation();
	if (HasAuthority())
	{
		// OnRep_Destination();
		ServerRPC_Q_Implementation();
	}
	else
	{
		ServerRPC_Q();
	}
}