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
	AttackRange->SetSphereRadius(180);

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

	GarenState = EGarenState::IDLE;

	// -----------------------

	CursorPlace = GetActorLocation();

	// -----------------------

	GarenAnim = Cast<UGarenAnimInstance>(this->GetMesh()->GetAnimInstance());
	check(GarenAnim);
	
	/* // UI부분
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

		// 가렌 Location -----------------------------------------------------------
		//Move_Garen();
		
		// 가렌 Rotation -----------------------------------------------------------
		//Turn_Garen();




	}
	else
	{
		// 가렌 Rotation -----------------------------------------------------------
		//Turn_Garen();

	}
	*/

	/* 로그
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
		// 마우스 우클릭 입력 - 이동 및 공격 타겟 설정
		EnhancedInputComponent->BindAction(Mouse_Right_Action, ETriggerEvent::Started, this, &AGaren::MouseRightClick);
		EnhancedInputComponent->BindAction(Mouse_Right_Action, ETriggerEvent::Triggered, this, &AGaren::MouseRightClick);

		// 마우스 좌클릭 입력
		EnhancedInputComponent->BindAction(Mouse_Left_Action, ETriggerEvent::Started, this, &AGaren::MouseLeftClick);

		// 키보드 Q 입력
		EnhancedInputComponent->BindAction(Q_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_Q);

		// 키보드 W 입력
		EnhancedInputComponent->BindAction(W_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_W);

		// 키보드 E 입력
		EnhancedInputComponent->BindAction(E_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_E);

		// 키보드 R 입력
		EnhancedInputComponent->BindAction(R_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_R);

		// 키보드 SpaceBar 입력
		EnhancedInputComponent->BindAction(SpaceBar_Action, ETriggerEvent::Started, this, &AGaren::KeyBoard_SpaceBar);

	}

}

void AGaren::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Target_Minion || Target_Champion || Target_Building) {
		return;
	}

	if (OtherActor->GetComponentByClass<UStateComponentBase>()) {
		// 해당 OtherActor를 공격하도록 함
		// 배열에 저장해야 할 듯

	}


}

void AGaren::NotifyActorEndOverlap(AActor* OtherActor)
{
	// 공격 범위에서 공격대상이 벗어난 경우, 공격대상을 따라감 - 미니언, 챔피언 불문

}


EGarenState AGaren::GetGarenState() const
{
	return EGarenState();
}

void AGaren::MouseRightClick(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse_Right"));

	GarenAnim->StopAllMontages(0.f);
	GarenAnim->AnimNotify_EndAttack_Garen(); // 다시 공격할 수 있도록 초기화

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	// 저장된 액터가 있다면 모두 삭제
	if (Target_Minion) {
		Target_Minion = nullptr;
	}
	if (Target_Champion) {
		Target_Champion = nullptr;
	}
	if (Target_Building) { 
		Target_Building = nullptr;
	}
	
	// 클린된 액터가 있다면 삭제
	/*if (MouseHitActor) {
		MouseHitActor = nullptr;
	}*/

	// 마우스로 클릭한 위치의 정보를 담기 위한 기능
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitInfo)) {
		// 클릭한 위치의 위치값을 저장
		CursorPlace = HitInfo.Location;
		CursorPlace.Z = 0.f;

		// 클릭한 위치의 액터를 저장 - 없다면 저장되지 않을 것
		MouseHitActor = HitInfo.GetActor();
		//UE_LOG(LogTemp, Warning, TEXT("MouseHitActor : %s"), *MouseHitActor->GetName());

		/* //클릭한 위치 값을 가져오는 로그 // 저장된 액터의 정보를 가져오는 로그
		UE_LOG(LogTemp, Warning, TEXT("CursorPlace: %s"), *CursorPlace.ToString());
		UE_LOG(LogTemp, Warning, TEXT("MouseHitActor: %s"), *MouseHitActor->GetName());
		*/

	}

	// 클릭한 위치의 액터를 분류해서 저장하는 기능 - StateComponent, FactionType, ObjectType으로 구분
	// 마우스로 클릭한 위치의 액터가 상태 컴포넌트를 가진다면
	if (MouseHitActor && MouseHitActor->GetComponentByClass<UStateComponentBase>()) {

		// 저장된 액터의 스테이트 컴포넌트를 저장
		UStateComponentBase* StateComponentBase = MouseHitActor->GetComponentByClass<UStateComponentBase>();

		// 만약 같은 팀이 아니라면
		if (StateComponentBase && StateComponentBase->GetFactionType() != StateComp_Garen->GetFactionType()) {

			// 각 액터의 종류에 따라 변수 저장
			if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::MINION || MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::SUPERMINION) {
				Target_Minion = Cast<AMinionBase>(MouseHitActor); // 공격대상이 미니언이라면

			}
			else if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::CHAMPION) {
				Target_Champion = Cast<ACharacter>(MouseHitActor); // 공격대상이 챔피언이라면

			}
			else if (MouseHitActor->GetComponentByClass<UStateComponentBase>()->GetObjectType() == EObjectType::BUILDING) {
				Target_Building = Cast<ABuilding_Base>(MouseHitActor); // 공격대상이 타워라면

			}

			// 로그 - 임시
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

	// 목표 위치 값과의 거리가 멀면
	if (MouseHitActor && FVector::Dist(this->GetActorLocation(), MouseHitActor->GetActorLocation()) > 0) {

		GarenState = EGarenState::MOVE;

	}
	else
	{
		GarenState = EGarenState::IDLE;

	}

	// 이동 함수 호출
	UE_LOG(LogTemp, Warning, TEXT("==========================================="));

}

void AGaren::MouseRightClick_Triggered(const FInputActionValue& value)
{
	


}

void AGaren::MouseLeftClick(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse_Left"));
	// 특정 스킬을 사용한 경우, 대상을 지정해야 할 때 사용한다. 


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

void AGaren::Move_Garen() // 가렌의 이동
{
	//UE_LOG(LogTemp, Warning, TEXT("Move_Success"));

	// 지정된 위치로 이동한다. 

	float Speed = 300;

	// 캐릭터의 현재 위치를 저장
	FVector ActorLocatoin = GetActorLocation();
	ActorLocatoin.Z = 0.f;
	// 지정된 위치의 방향을 설정
	FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

	// 캐릭터의 위치를 이동
	AddActorWorldOffset(Direction * Speed * GetWorld()->GetDeltaSeconds());

	// 지정된 타겟이 있을 경우, 거리 안에 들어오면 Attack으로 전환
	if (Target_Minion) {
		//오차범위 10 이내이면 상태를 ATTACK로 전환
		if (FVector::Dist(ActorLocatoin, Target_Minion->GetActorLocation()) <= 250) {
			GarenState = EGarenState::ATTACK;
		}
		return;

	}
	else if (Target_Champion) {
		//오차범위 10 이내이면 상태를 ATTACK로 전환
		if (FVector::Dist(ActorLocatoin, Target_Champion->GetActorLocation()) <= 250) {
			GarenState = EGarenState::ATTACK;
		}
		return;

	}
	else if (Target_Building) {
		//오차범위 10 이내이면 상태를 ATTACK로 전환
		if (FVector::Dist(ActorLocatoin, Target_Building->GetActorLocation()) <= 250) {
			GarenState = EGarenState::ATTACK;
		}
		return;

	}

	// 지정한 위치 도착시, 오차범위 10 이내이면 상태를 IDLE로 전환
	if (FVector::Dist(ActorLocatoin, CursorPlace) <= 10) {
		GarenState = EGarenState::IDLE;
	}

}

void AGaren::Turn_Garen() // 가렌의 회전 
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
	CurrentRotation = FMath::RInterpTo(CurrentRotation, TurnRotation, GetWorld()->GetDeltaSeconds(), 10); // tick마다 10씩 이동

	// 최종 회전 설정 - Tick으로 갱신
	SetActorRotation(CurrentRotation);

}

void AGaren::Attack_Normal_Garen()
{
	// 공격 대상이 지정되었을 때, 대상이 공격범위에 있지 않다면 대상이 있는 방향으로 대상이 들어올 때까지 이동한다.
	// 공격 대상이 지정되었을 때, 대상이 공격범위에 있다면 대상을 공격한다. 

	// 지정된 타겟의 스테이터스 컴포넌트를 가져와 데미지 주는 기능
	if (Target_Minion ) {
		UStateComponentBase* stateComp = Target_Minion->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(StateComp_Garen->GetAttackDamage(), StateComp_Garen->GetAbilityPower());

		//UE_LOG(LogTemp, Warning, TEXT("SuccessAttack"));

		

		// 상대방이 죽었을 때의 상태를 알 수 있도록 해야 함 - 공격을 멈추고 state를 idle로 바꾸도록 해야 함
		//if(Target_Minion->GetEnemyState() == )

	}
	else if (Target_Champion) {
		UStateComponentBase* stateComp = Target_Champion->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(StateComp_Garen->GetAttackDamage(), StateComp_Garen->GetAbilityPower());

		//UE_LOG(LogTemp, Warning, TEXT("SuccessAttack"));



		// 상대방이 죽었을 때의 상태를 알 수 있도록 해야 함 - 공격을 멈추고 state를 idle로 바꾸도록 해야 함
		//if(Target_Minion->GetEnemyState() == )

	}
	else if (Target_Building) {
		UStateComponentBase* stateComp = Target_Building->GetComponentByClass<UStateComponentBase>();

		stateComp->ApplyDamage(StateComp_Garen->GetAttackDamage(), StateComp_Garen->GetAbilityPower());

		//UE_LOG(LogTemp, Warning, TEXT("SuccessAttack"));



		// 상대방이 죽었을 때의 상태를 알 수 있도록 해야 함 - 공격을 멈추고 state를 idle로 바꾸도록 해야 함
		//if(Target_Minion->GetEnemyState() == )

	}
}

/*
이동 - 마우스로 클릭한 지점의 위치값을 가져온다.
해당 위치 값으로 캐릭터가 이동하도록 한다.
만약 부딪힌 지점에 챔피언, 미니언, 타워가 있다면, 이를 공격 타겟으로 저장하고 그 장소로 이동한다.
	- 해당 유닛이 Destroy 혹은 사라진 경우에는 타겟에서 해제한다.
	- 만약 해당 지점에 도착하기 이전에 다른 지점을 클릭한 경우에는 타겟을 삭제하고, 위를 반복한다.



공격기능(평타) 
타겟이 정해져 있는 경우 - 마우스 우클릭
	- 해당 타겟이 공격 거리 안에 있다면 공격한다.-> AttackRange과 충돌 중이라면 상태를 Attack으로 전환한다. 
		- 해당 타겟이 살아있으면서 공격 거리를 이탈했다면 해당 타겟을 쫓아간다. 
	- 해당 타겟이 공격 거리 안에 없다면 해당 타겟으로 이동한다.
		- 만약 타겟이 공격 거리 안에 들어왔다면 이동을 멈추고 공격을 한다. 



타겟이 정해져 있지 않은 경우
	- 캐릭터가 IDLE상태일 때, 적이 공격 거리 안에 들어온 경우, 타겟으로 삼고 공격을 한다. 

*/

void AGaren::Damaged()
{
	// 피격시 가렌의 체력이 남아있을 때


}

void AGaren::Die()
{	
	// 피격시 가렌의 체력이 0이하 일 때
	GarenAnim->PlayANM_Dead();
	GarenState = EGarenState::DEAD;


	// 죽고 나서 죽은 상태로 애니메이션을 고정하도록 해야 함

	// 죽고 난 후에는 마우스의 클릭에 캐릭터가 반응하지 않도록 해야 함

}
