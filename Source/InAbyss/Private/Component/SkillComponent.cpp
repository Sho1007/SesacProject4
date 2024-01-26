// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SkillComponent.h"

#include "EnhancedInputComponent.h"
#include "AnimInstance/EzrealAnimInstance.h"
#include "GameFramework/Character.h"

#include "Component/FSMComponent.h" 

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USkillComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(IA_Q, ETriggerEvent::Started, this, &USkillComponent::QStarted);
}

void USkillComponent::QStarted(const FInputActionValue& Value)
{
	// Destination = GetActorLocation();
	if (GetOwner()->HasAuthority())
	{
		// OnRep_Destination();
		ServerRPC_Q_Implementation();
	}
	else
	{
		ServerRPC_Q();
	}
}

void USkillComponent::ServerRPC_Q_Implementation()
{
	MultiRPC_Q();
}

void USkillComponent::MultiRPC_Q_Implementation()
{
}

// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// AnimInstance Init
	AnimInstance = Cast<UEzrealAnimInstance>(GetOwner<ACharacter>()->GetMesh()->GetAnimInstance());
	if (GetOwner()->HasAuthority() == true)
	{
		FSMComponent = GetOwner()->GetComponentByClass<UFSMComponent>();
	}
}

// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}