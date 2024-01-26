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
#include "NiagaraComponent.h"


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
	AttackRange->SetSphereRadius(230);

	// 이펙트 컴포넌트
	NSComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSComp"));
	NSComp->SetupAttachment(RootComponent);
	NSComp->SetRelativeScale3D(FVector(1));

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

	switch (GarenState)
	{
	case EGarenState::IDLE:

		//GarenAnim->PlayANM_Idle();


		// 공격 범위 안에 적이 들어오면 공격으로 전환

		break;
	case EGarenState::MOVE:

		//GarenAnim->PlayANM_Move();
		Move_Garen();

		if (GarenAnim->bIsSkilling_E == true) {
			break;

		}

		// 지정된 타겟이 있을 경우, 거리 안에 들어오면 Attack으로 전환
		if (Target_Minion && GarenState != EGarenState::ATTACK) {
			//오차범위 이내이면 상태를 ATTACK로 전환
			if (FVector::Dist(this->GetActorLocation(), Target_Minion->GetActorLocation()) <= 250) {

				GarenState = EGarenState::ATTACK;
			}

		}
		else if (Target_Champion) {
			//오차범위 10 이내이면 상태를 ATTACK로 전환
			if (FVector::Dist(this->GetActorLocation(), Target_Champion->GetActorLocation()) <= 250) {
				GarenState = EGarenState::ATTACK;
			}

		}
		else if (Target_Building) {
			//오차범위 10 이내이면 상태를 ATTACK로 전환
			if (FVector::Dist(this->GetActorLocation(), Target_Building->GetActorLocation()) <= 400) {
				GarenState = EGarenState::ATTACK;
			}

		}

		break;
	case EGarenState::ATTACK:

		if (Target_Minion && Target_Minion->GetComponentByClass<UStateComponentBase>()->IsDead() == true) {
			GarenState = EGarenState::IDLE;

		}
		else if (Target_Champion && Target_Champion->GetComponentByClass<UStateComponentBase>()->IsDead() == true) {
			GarenState = EGarenState::IDLE;

		}
		else if (Target_Building && Target_Building->GetComponentByClass<UStateComponentBase>()->IsDead() == true) {
			GarenState = EGarenState::IDLE;

		}

		GarenAnim->PlayANM_Attack();

		// 지정된 적이 공격 범위 밖으로 나가면 적을 쫓아감 - move로 전환

		break;
	}

	


	if (bIs_R_Move) {

		// 이동 기능
		//Move_Garen();
		GarenState = EGarenState::MOVE;

		if (FVector::Dist(GetActorLocation(), Target_Champion->GetActorLocation()) <= 500) { // 대상이 거리 안에 있다면

			bIs_R_Move = false;

			GarenAnim->PlayANM_R();

			/*
			if (Target_Champion->GetComponentByClass<UStateComponentBase>()->IsDead() == false) {
				GarenAnim->StopAllMontages(0.f);
				GarenState = EGarenState::MOVE;

			}
			*/
		}

	}

	//Turn_Garen();

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

	if (OtherActor->GetComponentByClass<UStateComponentBase>()) {


		// 공격 범위에 들어온 액터를 배열에 저장
		Targets_Attack.Add(OtherActor);

		
		// 해당 OtherActor를 공격하도록 함

		/*
		for (AActor* Target : Targets_Attack) {
			UE_LOG(LogTemp, Warning, TEXT("Begin : Target Actor: %s"), *Target->GetName());
		}
		*/
	}

}

void AGaren::NotifyActorEndOverlap(AActor* OtherActor)
{
	// 공격 범위에서 공격대상이 벗어난 경우, 공격대상을 따라감 - 미니언, 챔피언 불문

	if (OtherActor->GetComponentByClass<UStateComponentBase>()) {
		// 배열에서 삭제
		Targets_Attack.Remove(OtherActor);

		/*
		for (AActor* Target : Targets_Attack) {
			UE_LOG(LogTemp, Warning, TEXT("End : Target Actor: %s"), *Target->GetName());
		}
		*/
	}

}


EGarenState AGaren::GetGarenState() const
{
	return EGarenState();
}

void AGaren::MouseRightClick(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse_Right"));
	if (GarenAnim->bIsSkilling_Q == true) { //  Q스킬 쓰는 중이라면 클릭 안됨
		return;
	}

	if (GarenAnim->bIsSkilling_R == true) { //  R스킬 쓰는 중이라면 클릭 안됨
		return;
	}

	if (GarenAnim->bIsSkilling_E == false)
	{
		GarenAnim->StopAllMontages(0.f);
		//GarenAnim->AnimNotify_EndAttack_Garen(); // 다시 공격할 수 있도록 초기화
		GarenAnim->bIsAttack_Garen = false;

	}

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

		// 만약 같은 팀이 아니고, 죽은 상태가 아니라면
		if (StateComponentBase && StateComponentBase->GetFactionType() != StateComp_Garen->GetFactionType() && StateComponentBase->IsDead() == false) {

			// 각 액터의 종류에 따라 변수 저장
			if (StateComponentBase->GetObjectType() == EObjectType::MINION || StateComponentBase->GetObjectType() == EObjectType::SUPERMINION) {
				Target_Minion = Cast<AMinionBase>(MouseHitActor); // 공격대상이 미니언이라면

			}
			else if (StateComponentBase->GetObjectType() == EObjectType::CHAMPION) {
				Target_Champion = Cast<ACharacter>(MouseHitActor); // 공격대상이 챔피언이라면

			}
			else if (StateComponentBase->GetObjectType() == EObjectType::BUILDING) {
				Target_Building = Cast<ABuilding_Base>(MouseHitActor); // 공격대상이 타워라면

			}

			/*
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
			*/

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

}

void AGaren::MouseRightClick_Triggered(const FInputActionValue& value)
{



}

void AGaren::MouseLeftClick(const FInputActionValue& value)
{
	// R스킬 활성화 시에만 사용할 수 있도록 함
	if (GarenAnim->bIsSkill_R) {

		R_Skill_Garen();

		return;
	}

	

}

void AGaren::KeyBoard_Q(const FInputActionValue& value)
{
	GarenAnim->bIsSkill_Q = true;

	// q를 누르면 먼저하던 일반 공격 애니메이션을 취소하고 q애니매이션을 호출함

	GarenAnim->bIsQMove_Garen = true;

	Speed = Speed * 1.3;

	// 공격을 안하고 있었다면 공격 대상이 지정되었을 때 q애니매이션을 호출함

}

void AGaren::KeyBoard_W(const FInputActionValue& value)
{
	//NSComp->SetVisibility(true);

	// 사용할 이펙트 선택
	NSComp->SetAsset(SkillEffect[0]);

	// W 스킬 이펙트 스폰
	if (NSComp) {
		NSComp->Activate();
	}

	// 현재 3초 뒤 자동으로 비활성화 되도록 설정되어 있음
	//NSComp->Deactivate();
}

void AGaren::KeyBoard_E(const FInputActionValue& value)
{

	GarenAnim->PlayANM_E();

}

void AGaren::KeyBoard_R(const FInputActionValue& value)
{

	// R스킬 활성화
	GarenAnim->bIsSkill_R = true;


	// 스킬 범위를 이펙트로 표시
	// 마우스 커서 변경

}

void AGaren::KeyBoard_SpaceBar(const FInputActionValue& value)
{

}

void AGaren::Move_Garen() // 가렌의 이동
{
	// 지정된 위치로 이동한다. 

	// 캐릭터의 현재 위치를 저장
	FVector ActorLocatoin = GetActorLocation();
	ActorLocatoin.Z = 0.f;
	// 지정된 위치의 방향을 설정
	FVector Direction = (CursorPlace - ActorLocatoin).GetSafeNormal();

	if (GarenAnim->bIsSkilling_R == true) { //  R스킬 쓰는 중이라면 클릭 안됨

		return;
	}

	// 캐릭터의 위치를 이동
	AddActorWorldOffset(Direction * Speed * GetWorld()->GetDeltaSeconds());

	Turn_Garen();

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
	if (Target_Minion) {
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

void AGaren::Q_Skill_Garen()
{

}

void AGaren::W_Skill_Garen()
{

}

void AGaren::E_Skill_Garen()
{

	for (AActor* Actor : Targets_Attack) {


		if (Actor->GetComponentByClass<UStateComponentBase>()) {

			UStateComponentBase* StateComp = Actor->GetComponentByClass<UStateComponentBase>();

			if (StateComp->GetObjectType() == EObjectType::CHAMPION || StateComp->GetObjectType() == EObjectType::MINION || StateComp->GetObjectType() == EObjectType::SUPERMINION) {

				if (FVector::Dist(GetActorLocation(), Actor->GetActorLocation()) <= 300 && StateComp->IsDead() == false) {
					StateComp->ApplyDamage(StateComp_Garen->GetAttackDamage(), StateComp_Garen->GetAbilityPower());

				}

			}

		}

	}

}

void AGaren::R_Skill_Garen()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	GarenState = EGarenState::IDLE;

	// 마우스로 클릭한 위치의 정보를 담기 위한 기능
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitInfo)) {

		CursorPlace = HitInfo.Location;
		CursorPlace.Z = 0.f;

		// 클릭한 대상이 스테이터스 컴포넌트가 없으면 종료
		if (HitInfo.GetActor()->GetComponentByClass<UStateComponentBase>() == nullptr) {
			return;
		}

		// 저장된 액터의 스테이트 컴포넌트를 저장
		UStateComponentBase* StateComponentBase = HitInfo.GetActor()->GetComponentByClass<UStateComponentBase>();

		// 클릭한 대상이 챔피언이라면 타겟으로 저장
		if (StateComponentBase->GetObjectType() == EObjectType::CHAMPION) {

			Target_Champion = Cast<ACharacter>(HitInfo.GetActor());

		}
		else { // 챔피언이 아니라면 종료

			HitInfo.Reset();
			StateComponentBase = nullptr;
			Target_Champion = nullptr;
			GarenAnim->bIsSkilling_R = false;
			GarenAnim->bIsSkill_R = false;

			return;
		}

		if (FVector::Dist(GetActorLocation(), Target_Champion->GetActorLocation()) <= 500) { // 대상이 거리 안에 있다면
			// 스킬 사용
			GarenAnim->PlayANM_R();

		}
		else if (FVector::Dist(GetActorLocation(), Target_Champion->GetActorLocation()) >= 500 && bIs_R_Move == false) { // 대상이 거리 안에 없다면
			// 대상이 있는 곳까지 이동

			bIs_R_Move = true;

		}

	}
}

void AGaren::R_Move()
{

}

void AGaren::Damaged()
{
	// 피격시 가렌의 체력이 남아있을 때


}

void AGaren::Die()
{
	// 피격시 가렌의 체력이 0이하 일 때
	//GarenAnim->PlayANM_Dead();
	GarenState = EGarenState::DEAD;

	// 죽고 나서 죽은 상태로 애니메이션을 고정하도록 해야 함

	// 죽고 난 후에는 마우스의 클릭에 캐릭터가 반응하지 않도록 해야 함

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