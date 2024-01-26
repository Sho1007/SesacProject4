// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ezreal.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/FSMComponent.h"
#include "Component/SkillComponent.h"
#include "Component/StateComponentBase.h"
#include "GameFramework/SpringArmComponent.h"

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

	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	SkillComponent->SetIsReplicated(true);
}

void AEzreal::BeginPlay()
{
	Super::BeginPlay();

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

void AEzreal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsLocallyControlled() == false) return;

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		FSMComponent->SetupPlayerInputComponent(EnhancedInputComponent);
		SkillComponent->SetupPlayerInputComponent(EnhancedInputComponent);
	}
}

void AEzreal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}