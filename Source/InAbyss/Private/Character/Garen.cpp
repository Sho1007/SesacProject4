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

	// 상태 컴포넌트
	StateComp_Garen = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComp_Garen"));
	StateComp_Garen->SetObjectType(EObjectType::CHAMPION);

	// 스프링 암 컴포넌트
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 2000.f;
	SpringArmComp->SetRelativeRotation(FRotator(-50, 0, 0));

	// 카메라 컴포넌트
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->FieldOfView = 55.f;

	// 공격 범위 콜리전
	AttackRange = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRange"));
	AttackRange->SetupAttachment(RootComponent);
	AttackRange->SetSphereRadius(130);

}

// Called when the game starts or when spawned
void AGaren::BeginPlay()
{
	Super::BeginPlay();

	// 키 바인딩
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
		SetActorLocation(FVector(TargetLocation.X, TargetLocation.Y, GetActorLocation().Z)); // 떨림을 없애기 위해 Z값은 고정
		*/

		// 가렌 Location -----------------------------------------------------------
		float Speed = 300;

		// 캐릭터의 현재 위치를 저장
		FVector ActorLocatoin = GetActorLocation();
		ActorLocatoin.Z = 0.f;
		// 지정된 위치의 방향을 설정
		FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

		// 캐릭터의 위치를 이동
		AddActorWorldOffset(Direction * Speed * DeltaTime);

		// 지정한 위치 도착시, 오차범위 10 이내이면 상태를 IDLE로 전환
		if (FVector::Dist(ActorLocatoin, CursorPlace) <= 10) {
			GarenState = EGarenState::IDLE;
		}

		// 가렌 Rotation -----------------------------------------------------------
		// 
		// 캐릭터의 Yaw값만 변화하면 됨
		FRotator TurnRotation = Direction.Rotation();
		TurnRotation.Pitch = 0.f;  // 방향 이동시 Pitch를 0으로 고정
		TurnRotation.Roll = 0.f;  // 방향 이동시 Pitch를 0으로 고정

		
		// 서서히 회전
		CurrentRotation = FMath::RInterpTo(CurrentRotation, TurnRotation, DeltaTime, 10);

		// 최종 회전 설정 - Tick으로 갱신
		SetActorRotation(CurrentRotation);

		/*
		SetActorRotation(TurnRotation);
		CurrentRotation = GetActorRotation();
		*/

		// 로그
		/*
		UE_LOG(LogTemp, Warning, TEXT("CurrentRotation: %s"), *CurrentRotation.ToString());

		UE_LOG(LogTemp, Warning, TEXT("GetActorRotation: %s"), *GetActorRotation().ToString());

		UE_LOG(LogTemp, Warning, TEXT("NewRotation: %s"), *TurnRotation.ToString());
		*/
	}
	else
	{

		// 캐릭터의 현재 위치를 저장
		FVector ActorLocatoin = GetActorLocation();
		ActorLocatoin.Z = 0.f;
		// 지정된 위치의 방향을 설정
		FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

		// 가렌 Rotation -----------------------------------------------------------
		// 캐릭터의 Yaw값만 변화하면 됨

		FRotator TurnRotation = Direction.Rotation();
		TurnRotation.Pitch = 0.f;  // 방향 이동시 Pitch를 0으로 고정
		TurnRotation.Roll = 0.f;  // 방향 이동시 Pitch를 0으로 고정

		// 서서히 회전
		CurrentRotation = FMath::RInterpTo(CurrentRotation, TurnRotation, DeltaTime, 10);
		
		// 최종 회전 설정 - Tick으로 갱신
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
		// 마우스 우클릭 입력
		EnhancedInputComponent->BindAction(Mouse_Right_Action, ETriggerEvent::Started, this, &AGaren::MouseRightClick);
		EnhancedInputComponent->BindAction(Mouse_Right_Action, ETriggerEvent::Triggered, this, &AGaren::MouseRightClick);

		// 마우스 좌클릭 입력
		EnhancedInputComponent->BindAction(Mouse_Left_Action, ETriggerEvent::Started, this, &AGaren::MouseLeftClick);

		// 키보드 Q 입력


		// 키보드 W 입력


		// 키보드 E 입력


		// 키보드 R 입력


		// 키보드 SpaceBar 입력


	}


}

void AGaren::NotifyActorBeginOverlap(AActor* OtherActor)
{

	// 공격 범위에 공격대상이 들어온 경우
	if (OtherActor == Target_Test) {

		// 일반공격 함수를 호출
		Attack_Normal_Garen();

	}

}

void AGaren::NotifyActorEndOverlap(AActor* OtherActor)
{

	// 공격 범위에서 공격대상이 벗어난 경우, 공격

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

		// 마우스로 클릭한 위치의 정보를 담기 위한 기능
		if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitInfo)) {
			// 클릭한 위치의 위치값을 저장
			CursorPlace = HitInfo.Location;
			CursorPlace.Z = 0.f;

			/* //클릭한 위치 값을 가져오는 로그
			UE_LOG(LogTemp, Warning, TEXT("CursorPlace: %s"), *CursorPlace.ToString());
			*/

			// 클릭한 위치의 액터를 저장 - 없다면 저장되지 않을 것
			MouseHitActor = HitInfo.GetActor();

			/* // 저장된 액터의 정보를 가져오는 로그
			UE_LOG(LogTemp, Warning, TEXT("MouseHitActor: %s"), *MouseHitActor->GetName());
			*/

			//if(MouseHitActor && MouseHitActor->GetComponentByClass<UStateComponentBase>() && MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetFactionType() != StateComp_Garen->GetFactionType())

			// 마우스로 클릭한 위치에 액터가 있고, 상태 컴포넌트를 가지며, 같은 팀이 아니라면
			if (MouseHitActor && MouseHitActor->GetComponentByClass<UStateComponentBase>()) {
				// 저장된 액터의 스테이트 컴포넌트를 저장
				UStateComponentBase* StateComponentBase = MouseHitActor->GetComponentByClass<UStateComponentBase>();
				// 같은 팀인지 판단하고, 만약 적이라면
				if (StateComponentBase && StateComponentBase->GetFactionType() != StateComp_Garen->GetFactionType()) {

					// 공격대상으로 지정 - 임시
					Target_Test = MouseHitActor;

					// 각 액터의 종류에 따라 변수 저장
					if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::MINION) {
						Target_Minion = Cast<AMinionBase>(MouseHitActor); // 공격대상이 미니언이라면

					}
					else if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::CHAMPION) {
						Target_Champion = Cast<ACharacter>(MouseHitActor); // 공격대상이 챔피언이라면

					}
					else if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::BUILDING) {
						Target_Building = Cast<ABuilding_Base>(MouseHitActor); // 공격대상이 타워라면

					}

				}

			}

		}

	}

	// 위치 값이 자신과 다르다면 
	if (CursorPlace != GetActorLocation()) {


		Move_Garen();

	}

	// 이동 함수 호출

}

void AGaren::MouseLeftClick(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse_Left"));

	// 특정 스킬을 사용한 경우, 대상을 지정해야 할 때 사용한다. 


}

void AGaren::Move_Garen()
{
	//UE_LOG(LogTemp, Warning, TEXT("Move_Success"));

	// 지정된 위치로 이동한다. 
	// 만약 중간에 장애물이 있다면 피해서 지정된 위치로 이동한다. 

	//UE_LOG(LogTemp, Warning, TEXT("CursorPlace: %s"), *CursorPlace.ToString());


	// 캐릭터가 이동해야 하는 방향을 바라보도록 만듦 - Rotation값 수정


	GarenState = EGarenState::MOVE;

}

void AGaren::aaa()
{
	
	// 가렌 Location -----------------------------------------------------------
	float Speed = 300;

	// 캐릭터의 현재 위치를 저장
	FVector ActorLocatoin = GetActorLocation();
	ActorLocatoin.Z = 0.f;
	// 지정된 위치의 방향을 설정
	FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

	// 캐릭터의 위치를 이동
	AddActorWorldOffset(Direction * Speed * GetWorld()->GetDeltaSeconds());

	// 지정한 위치 도착시, 오차범위 10 이내이면 상태를 IDLE로 전환
	if (FVector::Dist(ActorLocatoin, CursorPlace) <= 10) {
		GarenState = EGarenState::IDLE;
	}

}

void AGaren::bbb()
{
	
	// 캐릭터의 현재 위치를 저장
	FVector ActorLocatoin = GetActorLocation();
	ActorLocatoin.Z = 0.f;
	// 지정된 위치의 방향을 설정
	FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();


	// 가렌 Rotation -----------------------------------------------------------
	// 캐릭터의 Yaw값만 변화하면 됨

	FRotator NewRotation = Direction.Rotation();
	NewRotation.Pitch = 0.f;  // 방향 이동시 Pitch를 0으로 고정
	NewRotation.Roll = 0.f;  // 방향 이동시 Pitch를 0으로 고정


	SetActorRotation(NewRotation);

	UE_LOG(LogTemp, Warning, TEXT("GetActorRotation: %s"), *GetActorRotation().ToString());

	UE_LOG(LogTemp, Warning, TEXT("NewRotation: %s"), *NewRotation.ToString());
}

void AGaren::Attack_Normal_Garen()
{
	// 공격 대상이 지정되었을 때, 대상이 공격범위에 있지 않다면 대상이 있는 방향으로 대상이 들어올 때까지 이동한다.

	// 공격 대상이 지정되었을 때, 대상이 공격범위에 있다면 대상을 공격한다. 



}

/*
이동 - 마우스로 클릭한 지점의 위치값을 가져온다.
해당 위치 값으로 캐릭터가 이동하도록 한다.
만약 부딪힌 지점에 챔피언, 미니언, 타워가 있다면, 이를 공격 타겟으로 저장하고 그 장소로 이동한다.
	- 해당 유닛이 Destroy 혹은 사라진 경우에는 타겟에서 해제한다.
	- 만약 해당 지점에 도착하기 이전에 다른 지점을 클릭한 경우에는 타겟을 삭제하고, 위를 반복한다.







*/



