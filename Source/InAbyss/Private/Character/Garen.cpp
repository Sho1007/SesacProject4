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
	AttackRange->SetSphereRadius(130);

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

	// -----------------------
	GarenState = EGarenState::IDLE;

	CursorPlace = GetActorLocation();

}

// Called every frame
void AGaren::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GarenState == EGarenState::MOVE) {

		/*
		FVector TargetLocation = GetActorLocation() + Direction * Speed * DeltaTime;
		SetActorLocation(FVector(TargetLocation.X, TargetLocation.Y, GetActorLocation().Z)); // ������ ���ֱ� ���� Z���� ����
		*/

		// ���� Location -----------------------------------------------------------
		float Speed = 300;

		// ĳ������ ���� ��ġ�� ����
		FVector ActorLocatoin = GetActorLocation();
		ActorLocatoin.Z = 0.f;
		// ������ ��ġ�� ������ ����
		FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

		// ĳ������ ��ġ�� �̵�
		AddActorWorldOffset(Direction * Speed * DeltaTime);

		// ������ ��ġ ������, �������� 10 �̳��̸� ���¸� IDLE�� ��ȯ
		if (FVector::Dist(ActorLocatoin, CursorPlace) <= 10) {
			GarenState = EGarenState::IDLE;
		}

		// ���� Rotation -----------------------------------------------------------
		// 
		// ĳ������ Yaw���� ��ȭ�ϸ� ��
		FRotator TurnRotation = Direction.Rotation();
		TurnRotation.Pitch = 0.f;  // ���� �̵��� Pitch�� 0���� ����
		TurnRotation.Roll = 0.f;  // ���� �̵��� Pitch�� 0���� ����

		
		// ������ ȸ��
		CurrentRotation = FMath::RInterpTo(CurrentRotation, TurnRotation, DeltaTime, 10);

		// ���� ȸ�� ���� - Tick���� ����
		SetActorRotation(CurrentRotation);

		/*
		SetActorRotation(TurnRotation);
		CurrentRotation = GetActorRotation();
		*/

		// �α�
		/*
		UE_LOG(LogTemp, Warning, TEXT("CurrentRotation: %s"), *CurrentRotation.ToString());

		UE_LOG(LogTemp, Warning, TEXT("GetActorRotation: %s"), *GetActorRotation().ToString());

		UE_LOG(LogTemp, Warning, TEXT("NewRotation: %s"), *TurnRotation.ToString());
		*/
	}
	else
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
		CurrentRotation = FMath::RInterpTo(CurrentRotation, TurnRotation, DeltaTime, 10);
		
		// ���� ȸ�� ���� - Tick���� ����
		SetActorRotation(CurrentRotation);

		/*
		SetActorRotation(TurnRotation);

		CurrentRotation = GetActorRotation();
		*/
	}






}

// Called to bind functionality to input
void AGaren::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent) {
		// ���콺 ��Ŭ�� �Է�
		EnhancedInputComponent->BindAction(Mouse_Right_Action, ETriggerEvent::Started, this, &AGaren::MouseRightClick);
		EnhancedInputComponent->BindAction(Mouse_Right_Action, ETriggerEvent::Triggered, this, &AGaren::MouseRightClick);

		// ���콺 ��Ŭ�� �Է�
		EnhancedInputComponent->BindAction(Mouse_Left_Action, ETriggerEvent::Started, this, &AGaren::MouseLeftClick);

		// Ű���� Q �Է�


		// Ű���� W �Է�


		// Ű���� E �Է�


		// Ű���� R �Է�


		// Ű���� SpaceBar �Է�


	}


}

void AGaren::NotifyActorBeginOverlap(AActor* OtherActor)
{

	// ���� ������ ���ݴ���� ���� ���
	if (OtherActor == Target_Test) {

		// �Ϲݰ��� �Լ��� ȣ��
		Attack_Normal_Garen();

	}

}

void AGaren::NotifyActorEndOverlap(AActor* OtherActor)
{

	// ���� �������� ���ݴ���� ��� ���, ����

}


EGarenState AGaren::GetGarenState() const
{
	return EGarenState();
}

void AGaren::MouseRightClick(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse_Right"));

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{

		// ���콺�� Ŭ���� ��ġ�� ������ ��� ���� ���
		if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitInfo)) {
			// Ŭ���� ��ġ�� ��ġ���� ����
			CursorPlace = HitInfo.Location;
			CursorPlace.Z = 0.f;

			/* //Ŭ���� ��ġ ���� �������� �α�
			UE_LOG(LogTemp, Warning, TEXT("CursorPlace: %s"), *CursorPlace.ToString());
			*/

			// Ŭ���� ��ġ�� ���͸� ���� - ���ٸ� ������� ���� ��
			MouseHitActor = HitInfo.GetActor();

			/* // ����� ������ ������ �������� �α�
			UE_LOG(LogTemp, Warning, TEXT("MouseHitActor: %s"), *MouseHitActor->GetName());
			*/

			//if(MouseHitActor && MouseHitActor->GetComponentByClass<UStateComponentBase>() && MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetFactionType() != StateComp_Garen->GetFactionType())

			// ���콺�� Ŭ���� ��ġ�� ���Ͱ� �ְ�, ���� ������Ʈ�� ������, ���� ���� �ƴ϶��
			if (MouseHitActor && MouseHitActor->GetComponentByClass<UStateComponentBase>()) {
				// ����� ������ ������Ʈ ������Ʈ�� ����
				UStateComponentBase* StateComponentBase = MouseHitActor->GetComponentByClass<UStateComponentBase>();
				// ���� ������ �Ǵ��ϰ�, ���� ���̶��
				if (StateComponentBase && StateComponentBase->GetFactionType() != StateComp_Garen->GetFactionType()) {

					// ���ݴ������ ���� - �ӽ�
					Target_Test = MouseHitActor;

					// �� ������ ������ ���� ���� ����
					if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::MINION) {
						Target_Minion = Cast<AMinionBase>(MouseHitActor); // ���ݴ���� �̴Ͼ��̶��

					}
					else if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::CHAMPION) {
						Target_Champion = Cast<ACharacter>(MouseHitActor); // ���ݴ���� è�Ǿ��̶��

					}
					else if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::BUILDING) {
						Target_Building = Cast<ABuilding_Base>(MouseHitActor); // ���ݴ���� Ÿ�����

					}

				}

			}

		}

	}

	// ��ġ ���� �ڽŰ� �ٸ��ٸ� 
	if (CursorPlace != GetActorLocation()) {


		Move_Garen();

	}

	// �̵� �Լ� ȣ��

}

void AGaren::MouseLeftClick(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse_Left"));

	// Ư�� ��ų�� ����� ���, ����� �����ؾ� �� �� ����Ѵ�. 


}

void AGaren::Move_Garen()
{
	//UE_LOG(LogTemp, Warning, TEXT("Move_Success"));

	// ������ ��ġ�� �̵��Ѵ�. 
	// ���� �߰��� ��ֹ��� �ִٸ� ���ؼ� ������ ��ġ�� �̵��Ѵ�. 

	//UE_LOG(LogTemp, Warning, TEXT("CursorPlace: %s"), *CursorPlace.ToString());


	// ĳ���Ͱ� �̵��ؾ� �ϴ� ������ �ٶ󺸵��� ���� - Rotation�� ����


	GarenState = EGarenState::MOVE;

}

void AGaren::aaa()
{
	
	// ���� Location -----------------------------------------------------------
	float Speed = 300;

	// ĳ������ ���� ��ġ�� ����
	FVector ActorLocatoin = GetActorLocation();
	ActorLocatoin.Z = 0.f;
	// ������ ��ġ�� ������ ����
	FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

	// ĳ������ ��ġ�� �̵�
	AddActorWorldOffset(Direction * Speed * GetWorld()->GetDeltaSeconds());

	// ������ ��ġ ������, �������� 10 �̳��̸� ���¸� IDLE�� ��ȯ
	if (FVector::Dist(ActorLocatoin, CursorPlace) <= 10) {
		GarenState = EGarenState::IDLE;
	}

}

void AGaren::bbb()
{
	
	// ĳ������ ���� ��ġ�� ����
	FVector ActorLocatoin = GetActorLocation();
	ActorLocatoin.Z = 0.f;
	// ������ ��ġ�� ������ ����
	FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();


	// ���� Rotation -----------------------------------------------------------
	// ĳ������ Yaw���� ��ȭ�ϸ� ��

	FRotator NewRotation = Direction.Rotation();
	NewRotation.Pitch = 0.f;  // ���� �̵��� Pitch�� 0���� ����
	NewRotation.Roll = 0.f;  // ���� �̵��� Pitch�� 0���� ����


	SetActorRotation(NewRotation);

	UE_LOG(LogTemp, Warning, TEXT("GetActorRotation: %s"), *GetActorRotation().ToString());

	UE_LOG(LogTemp, Warning, TEXT("NewRotation: %s"), *NewRotation.ToString());
}

void AGaren::Attack_Normal_Garen()
{
	// ���� ����� �����Ǿ��� ��, ����� ���ݹ����� ���� �ʴٸ� ����� �ִ� �������� ����� ���� ������ �̵��Ѵ�.

	// ���� ����� �����Ǿ��� ��, ����� ���ݹ����� �ִٸ� ����� �����Ѵ�. 



}

/*
�̵� - ���콺�� Ŭ���� ������ ��ġ���� �����´�.
�ش� ��ġ ������ ĳ���Ͱ� �̵��ϵ��� �Ѵ�.
���� �ε��� ������ è�Ǿ�, �̴Ͼ�, Ÿ���� �ִٸ�, �̸� ���� Ÿ������ �����ϰ� �� ��ҷ� �̵��Ѵ�.
	- �ش� ������ Destroy Ȥ�� ����� ��쿡�� Ÿ�ٿ��� �����Ѵ�.
	- ���� �ش� ������ �����ϱ� ������ �ٸ� ������ Ŭ���� ��쿡�� Ÿ���� �����ϰ�, ���� �ݺ��Ѵ�.







*/



