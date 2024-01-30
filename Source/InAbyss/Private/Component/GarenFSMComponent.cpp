// Fill out your copyright notice in the Description page of Project Settings.
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>


#include "Component/GarenFSMComponent.h"
#include "Character/Garen.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "Component/GarenSkillComponent.h"
#include "Component/StateComponentBase.h"
#include "AnimInstance/GarenAnimInstance.h"
#include "Component/FSMComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputTriggers.h>
/*x
UGarenFSMComponent::UGarenFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UGarenFSMComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}

void UGarenFSMComponent::BeginPlay()
{
	Super::BeginPlay();


	Owner = GetOwner<AGaren>();
	AnimInstance = Cast<UGarenAnimInstance>(Owner->GetMesh()->GetAnimInstance());

	if (Owner->HasAuthority() == true)
	{
		SetIsReplicated(true);
	}

	// Find Other Component
	StateComponent = Owner->GetComponentByClass<UStateComponentBase>();
	SkillComponent = Owner->GetComponentByClass<UGarenSkillComponent>();

	if (Owner->IsLocallyControlled())
	{
		PlayerController = Owner->GetController<APlayerController>();
	}



	GarenState = EGarenState::IDLE;


	//CursorPlace = GetActorLocation();


	GarenAnim = Cast<UGarenAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	check(GarenAnim);
	
}

void UGarenFSMComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (GetOwner<AGaren>()->IsLocallyControlled() == false) return;
	EnhancedInputComponent->BindAction(IA_RightClick, ETriggerEvent::Started, this, &UFSMComponent::RightClickStarted);

}


EGarenState UGarenFSMComponent::GetGarenState() const
{
	return EGarenState();
}
*/