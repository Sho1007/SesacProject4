// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Garen.h"
#include "Component/StateComponentBase.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include "GameFramework/PlayerController.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include "Enemy/MinionBase.h"
#include "Building/Building_Base.h"
#include "AnimInstance/GarenAnimInstance.h"


// Sets default values
AGaren::AGaren()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���� ������Ʈ
	StateComp_Garen = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComp_Garen"));
	StateComp_Garen->SetObjectType(EObjectType::CHAMPION);

	// ������ �� ������Ʈ
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 2000.f;
	SpringArmComp->SetRelativeRotation(FRotator(-50, 0, 0));

	// ī�޶� ������Ʈ
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->FieldOfView = 55.f;

	// ���� ���� �ݸ���
	AttackRange = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRange"));
	AttackRange->SetupAttachment(RootComponent);
	AttackRange->SetSphereRadius(180);

}

// Called when the game starts or when spawned
void AGaren::BeginPlay()
{
	Super::BeginPlay();

	// Ű ���ε�
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
	}
	// -----------------------

	GarenState = EGarenState::IDLE;

	// -----------------------

	CursorPlace = GetActorLocation();

	// -----------------------

	GarenAnim = Cast<UGarenAnimInstance>(this->GetMesh()->GetAnimInstance());
	check(GarenAnim);
	
	/* // UI�κ�
	if (UHealthBarWidgetBase* HealthBarWidget = Cast<UHealthBarWidgetBase>(HealthBarWidgetComponent->GetWidget()))
	{
		HealthBarWidget->InitStateComponent(StateComponent);
	}
	*/
}

// Called every frame
void AGaren::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	if (GarenState == EGarenState::MOVE) {

		// ���� Location -----------------------------------------------------------
		//Move_Garen();
		
		// ���� Rotation -----------------------------------------------------------
		//Turn_Garen();




	}
	else
	{
		// ���� Rotation -----------------------------------------------------------
		//Turn_Garen();

	}
	*/

	/* �α�
	UE_LOG(LogTemp, Warning, TEXT("CurrentRotation: %s"), *CurrentRotation.ToString());

	UE_LOG(LogTemp, Warning, TEXT("GetActorRotation: %s"), *GetActorRotation().ToString());

	UE_LOG(LogTemp, Warning, TEXT("NewRotation: %s"), *TurnRotation.ToString());
	*/

	switch (GarenState)
	{
	case EGarenState::IDLE:
		
		//GarenAnim->PlayANM_Idle();

		Turn_Garen();
		break;
	case EGarenState::MOVE:

		//GarenAnim->PlayANM_Move();
		Move_Garen();

		Turn_Garen();
		break;
	case EGarenState::ATTACK:

		GarenAnim->PlayANM_Attack();

		Turn_Garen();

		break;
	/*case EGarenState::DEAD:

		GarenAnim->PlayANM_Dead();

		Turn_Garen();
		break;*/
	}


}

// Called to bind functionality to input
void AGaren::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent) {
		// ���콺 ��Ŭ�� �Է� - �̵� �� ���� Ÿ�� ����
		EnhancedInputComponent->BindAction(Mouse_Right_Action, ETriggerEvent::Started, this, &AGaren::MouseRightClick);
		EnhancedInputComponent->BindAction(Mouse_Right_Action, ETriggerEvent::Triggered, this, &AGaren::MouseRightClick);

		// ���콺 ��Ŭ�� �Է�
		EnhancedInputComponent->BindAction(Mouse_Left_Action, ETriggerEvent::Started, this, &AGaren::MouseLeftClick);

		// Ű���� Q �Է�
		EnhancedInputComponent->BindAction(Q_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_Q);

		// Ű���� W �Է�
		EnhancedInputComponent->BindAction(W_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_W);

		// Ű���� E �Է�
		EnhancedInputComponent->BindAction(E_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_E);

		// Ű���� R �Է�
		EnhancedInputComponent->BindAction(R_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_R);

		// Ű���� SpaceBar �Է�
		EnhancedInputComponent->BindAction(SpaceBar_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_SpaceBar);

	}

}

void AGaren::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Target_Minion || Target_Champion || Target_Building) {
		return;
	}

	if (OtherActor->GetComponentByClass<UStateComponentBase>()) {
		// �ش� OtherActor�� �����ϵ��� ��
		// �迭�� �����ؾ� �� ��

	}


}

void AGaren::NotifyActorEndOverlap(AActor* OtherActor)
{
	// ���� �������� ���ݴ���� ��� ���, ���ݴ���� ���� - �̴Ͼ�, è�Ǿ� �ҹ�

}


EGarenState AGaren::GetGarenState() const
{
	return EGarenState();
}

void AGaren::MouseRightClick(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse_Right"));

	GarenAnim->StopAllMontages(0.f);
	GarenAnim->AnimNotify_EndAttack_Garen(); // �ٽ� ������ �� �ֵ��� �ʱ�ȭ

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	// ����� ���Ͱ� �ִٸ� ��� ����
	if (Target_Minion) {
		Target_Minion = nullptr;
	}
	if (Target_Champion) {
		Target_Champion = nullptr;
	}
	if (Target_Building) { 
		Target_Building = nullptr;
	}
	
	// Ŭ���� ���Ͱ� �ִٸ� ����
	/*if (MouseHitActor) {
		MouseHitActor = nullptr;
	}*/

	// ���콺�� Ŭ���� ��ġ�� ������ ��� ���� ���
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitInfo)) {
		// Ŭ���� ��ġ�� ��ġ���� ����
		CursorPlace = HitInfo.Location;
		CursorPlace.Z = 0.f;

		// Ŭ���� ��ġ�� ���͸� ���� - ���ٸ� ������� ���� ��
		MouseHitActor = HitInfo.GetActor();
		//UE_LOG(LogTemp, Warning, TEXT("MouseHitActor : %s"), *MouseHitActor->GetName());

		/* //Ŭ���� ��ġ ���� �������� �α� // ����� ������ ������ �������� �α�
		UE_LOG(LogTemp, Warning, TEXT("CursorPlace: %s"), *CursorPlace.ToString());
		UE_LOG(LogTemp, Warning, TEXT("MouseHitActor: %s"), *MouseHitActor->GetName());
		*/

	}

	// Ŭ���� ��ġ�� ���͸� �з��ؼ� �����ϴ� ��� - StateComponent, FactionType, ObjectType���� ����
	// ���콺�� Ŭ���� ��ġ�� ���Ͱ� ���� ������Ʈ�� �����ٸ�
	if (MouseHitActor && MouseHitActor->GetComponentByClass<UStateComponentBase>()) {

		// ����� ������ ������Ʈ ������Ʈ�� ����
		UStateComponentBase* StateComponentBase = MouseHitActor->GetComponentByClass<UStateComponentBase>();

		// ���� ���� ���� �ƴ϶��
		if (StateComponentBase && StateComponentBase->GetFactionType() != StateComp_Garen->GetFactionType()) {

			// �� ������ ������ ���� ���� ����
			if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::MINION || MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::SUPERMINION) {
				Target_Minion = Cast<AMinionBase>(MouseHitActor); // ���ݴ���� �̴Ͼ��̶��

			}
			else if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::CHAMPION) {
				Target_Champion = Cast<ACharacter>(MouseHitActor); // ���ݴ���� è�Ǿ��̶��

			}
			else if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::BUILDING) {
				Target_Building = Cast<ABuilding_Base>(MouseHitActor); // ���ݴ���� Ÿ�����

			}

			// �α� - �ӽ�
			if (Target_Minion) {
				UE_LOG(LogTemp, Warning, TEXT("Target_Minion : %s"), *Target_Minion->GetName());
			}
			if (Target_Champion) {
				UE_LOG(LogTemp, Warning, TEXT("Target_Champion : %s"), *Target_Champion->GetName());
			}
			if (Target_Building) {
				UE_LOG(LogTemp, Warning, TEXT("Target_Building : %s"), *Target_Building->GetName());

			}
		}

	}

	// ��ǥ ��ġ ������ �Ÿ��� �ָ�
	if (MouseHitActor && FVector::Dist(this->GetActorLocation(), MouseHitActor->GetActorLocation()) > 0) {

		GarenState = EGarenState::MOVE;

	}
	else
	{
		GarenState = EGarenState::IDLE;

	}

	// �̵� �Լ� ȣ��
	UE_LOG(LogTemp, Warning, TEXT("==========================================="));

}

void AGaren::MouseRightClick_Triggered(const FInputActionValue& value)
{
	


}

void AGaren::MouseLeftClick(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse_Left"));
	// Ư�� ��ų�� ����� ���, ����� �����ؾ� �� �� ����Ѵ�. 


}

void AGaren::KeyBoard_Q(const FInputActionValue& value)
{
	//GarenAnim->bIsSkill_Q = true;

	UE_LOG(LogTemp,Warning, TEXT("Success Call"));
	GarenAnim->PlayANM_Q();
}

void AGaren::KeyBoard_W(const FInputActionValue& value)
{
}

void AGaren::KeyBoard_E(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Success Call"));

	GarenAnim->PlayANM_E();
}

void AGaren::KeyBoard_R(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Success Call"));

	GarenAnim->PlayANM_R();
}

void AGaren::KeyBoard_SpaceBar(const FInputActionValue& value)
{
}

void AGaren::Move_Garen() // ������ �̵�
{
	//UE_LOG(LogTemp, Warning, TEXT("Move_Success"));

	// ������ ��ġ�� �̵��Ѵ�. 

	float Speed = 300;

	// ĳ������ ���� ��ġ�� ����
	FVector ActorLocatoin = GetActorLocation();
	ActorLocatoin.Z = 0.f;
	// ������ ��ġ�� ������ ����
	FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

	// ĳ������ ��ġ�� �̵�
	AddActorWorldOffset(Direction * Speed * GetWorld()->GetDeltaSeconds());

	// ������ Ÿ���� ���� ���, �Ÿ� �ȿ� ������ Attack���� ��ȯ
	if (Target_Minion) {
		//�������� 10 �̳��̸� ���¸� ATTACK�� ��ȯ
		if (FVector::Dist(ActorLocatoin, Target_Minion->GetActorLocation()) <= 250) {
			GarenState = EGarenState::ATTACK;
		}
		return;

	}
	else if (Target_Champion) {
		//�������� 10 �̳��̸� ���¸� ATTACK�� ��ȯ
		if (FVector::Dist(ActorLocatoin, Target_Champion->GetActorLocation()) <= 250) {
			GarenState = EGarenState::ATTACK;
		}
		return;

	}
	else if (Target_Building) {
		//�������� 10 �̳��̸� ���¸� ATTACK�� ��ȯ
		if (FVector::Dist(ActorLocatoin, Target_Building->GetActorLocation()) <= 250) {
			GarenState = EGarenState::ATTACK;
		}
		return;

	}

	// ������ ��ġ ������, �������� 10 �̳��̸� ���¸� IDLE�� ��ȯ
	if (FVector::Dist(ActorLocatoin, CursorPlace) <= 10) {
		GarenState = EGarenState::IDLE;
	}

}

void AGaren::Turn_Garen() // ������ ȸ�� 
{

	// ĳ������ ���� ��ġ�� ����
	FVector ActorLocatoin = GetActorLocation();
	ActorLocatoin.Z = 0.f;
	// ������ ��ġ�� ������ ����
	FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

	// ���� Rotation -----------------------------------------------------------
	// ĳ������ Yaw���� ��ȭ�ϸ� ��

	FRotator TurnRotation = Direction.Rotation();
	TurnRotation.Pitch = 0.f;  // ���� �̵��� Pitch�� 0���� ����
	TurnRotation.Roll = 0.f;  // ���� �̵��� Pitch�� 0���� ����

	// ������ ȸ��
	CurrentRotation = FMath::RInterpTo(CurrentRotation, TurnRotation, GetWorld()->GetDeltaSeconds(), 10); // tick���� 10�� �̵�

	// ���� ȸ�� ���� - Tick���� ����
	SetActorRotation(CurrentRotation);

}

void AGaren::Attack_Normal_Garen()
{
	// ���� ����� �����Ǿ��� ��, ����� ���ݹ����� ���� �ʴٸ� ����� �ִ� �������� ����� ���� ������ �̵��Ѵ�.
	// ���� ����� �����Ǿ��� ��, ����� ���ݹ����� �ִٸ� ����� �����Ѵ�. 

	// ������ Ÿ���� �������ͽ� ������Ʈ�� ������ ������ �ִ� ���
	if (Target_Minion ) {
		UStateComponentBase* stateComp = Target_Minion->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(StateComp_Garen->GetAttackDamage(), StateComp_Garen->GetAbilityPower());

		//UE_LOG(LogTemp, Warning, TEXT("SuccessAttack"));

		

		// ������ �׾��� ���� ���¸� �� �� �ֵ��� �ؾ� �� - ������ ���߰� state�� idle�� �ٲٵ��� �ؾ� ��
		//if(Target_Minion->GetEnemyState() == )

	}
	else if (Target_Champion) {
		UStateComponentBase* stateComp = Target_Champion->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(StateComp_Garen->GetAttackDamage(), StateComp_Garen->GetAbilityPower());

		//UE_LOG(LogTemp, Warning, TEXT("SuccessAttack"));



		// ������ �׾��� ���� ���¸� �� �� �ֵ��� �ؾ� �� - ������ ���߰� state�� idle�� �ٲٵ��� �ؾ� ��
		//if(Target_Minion->GetEnemyState() == )

	}
	else if (Target_Building) {
		UStateComponentBase* stateComp = Target_Building->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(StateComp_Garen->GetAttackDamage(), StateComp_Garen->GetAbilityPower());

		//UE_LOG(LogTemp, Warning, TEXT("SuccessAttack"));



		// ������ �׾��� ���� ���¸� �� �� �ֵ��� �ؾ� �� - ������ ���߰� state�� idle�� �ٲٵ��� �ؾ� ��
		//if(Target_Minion->GetEnemyState() == )

	}
}

/*
�̵� - ���콺�� Ŭ���� ������ ��ġ���� �����´�.
�ش� ��ġ ������ ĳ���Ͱ� �̵��ϵ��� �Ѵ�.
���� �ε��� ������ è�Ǿ�, �̴Ͼ�, Ÿ���� �ִٸ�, �̸� ���� Ÿ������ �����ϰ� �� ��ҷ� �̵��Ѵ�.
	- �ش� ������ Destroy Ȥ�� ����� ��쿡�� Ÿ�ٿ��� �����Ѵ�.
	- ���� �ش� ������ �����ϱ� ������ �ٸ� ������ Ŭ���� ��쿡�� Ÿ���� �����ϰ�, ���� �ݺ��Ѵ�.



���ݱ��(��Ÿ) 
Ÿ���� ������ �ִ� ��� - ���콺 ��Ŭ��
	- �ش� Ÿ���� ���� �Ÿ� �ȿ� �ִٸ� �����Ѵ�.-> AttackRange�� �浹 ���̶�� ���¸� Attack���� ��ȯ�Ѵ�. 
		- �ش� Ÿ���� ��������鼭 ���� �Ÿ��� ��Ż�ߴٸ� �ش� Ÿ���� �Ѿư���. 
	- �ش� Ÿ���� ���� �Ÿ� �ȿ� ���ٸ� �ش� Ÿ������ �̵��Ѵ�.
		- ���� Ÿ���� ���� �Ÿ� �ȿ� ���Դٸ� �̵��� ���߰� ������ �Ѵ�. 



Ÿ���� ������ ���� ���� ���
	- ĳ���Ͱ� IDLE������ ��, ���� ���� �Ÿ� �ȿ� ���� ���, Ÿ������ ��� ������ �Ѵ�. 

*/

void AGaren::Damaged()
{
	// �ǰݽ� ������ ü���� �������� ��


}

void AGaren::Die()
{	
	// �ǰݽ� ������ ü���� 0���� �� ��
	GarenAnim->PlayANM_Dead();
	GarenState = EGarenState::DEAD;


	// �װ� ���� ���� ���·� �ִϸ��̼��� �����ϵ��� �ؾ� ��

	// �װ� �� �Ŀ��� ���콺�� Ŭ���� ĳ���Ͱ� �������� �ʵ��� �ؾ� ��

}
