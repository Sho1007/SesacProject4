// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SkillComponent.h"

#include "EnhancedInputComponent.h"
#include "AnimInstance/EzrealAnimInstance.h"
#include "GameFramework/Character.h"

#include "Component/FSMComponent.h" 
#include "InAbyss/InAbyss.h"
#include "Net/UnrealNetwork.h"

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
	PRINTLOG(TEXT(""));
	// Destination = GetActorLocation();
	FSMComponent->PrepareSkill();
}

void USkillComponent::Q()
{
	bIsSkilling = true;
	ServerRPC_Q();
}

void USkillComponent::FireQ()
{
}

void USkillComponent::ServerRPC_Q_Implementation()
{
	PRINTLOG(TEXT(""));
	bIsSkilling = true;
	MultiRPC_Q();
}

void USkillComponent::MultiRPC_Q_Implementation()
{
	AnimInstance->PlayQMontage();
}

void USkillComponent::ServerRPC_EndSkill_Implementation()
{
	bIsSkilling = false;
	OnRep_IsSkilling();
	FSMComponent->EndSkill();
}

void USkillComponent::OnRep_IsSkilling()
{
	PRINTLOG(TEXT("%d"), bIsSkilling);
}

// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// AnimInstance Init
	AnimInstance = Cast<UEzrealAnimInstance>(GetOwner<ACharacter>()->GetMesh()->GetAnimInstance());
	FSMComponent = GetOwner()->GetComponentByClass<UFSMComponent>();
}

void USkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USkillComponent, bIsSkilling);
}

// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USkillComponent::IsSkilling() const
{
	return bIsSkilling;
}
