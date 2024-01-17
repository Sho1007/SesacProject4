// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Garen.h"
#include "Component/StateComponentBase.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>


// Sets default values
AGaren::AGaren()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateComp_Garen = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComp_Garen"));
	StateComp_Garen->SetObjectType(EObjectType::CHAMPION);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 2000.f;
	SpringArmComp->SetRelativeRotation(FRotator(-50, 0, 0));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->FieldOfView = 55.f;

}

// Called when the game starts or when spawned
void AGaren::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGaren::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGaren::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("IA_MoveAndAttack", IE_Pressed, this, &AGaren::MouseRightClick);

}



/*
이동 - 마우스로 클릭한 지점에 라인트레이스를 쏜다.
라인이 부딪힌 지점의 위치 값을 가져와서 캐릭터가 이동하도록 한다.
만약 부딪힌 지점에 미니언, 타워가 있다면, 해당 액터가 있다면, 이를 공격 타겟으로 저장하고 그 장소로 이동한다.
	- 해당 유닛이 Destroy 혹은 사라진 경우에는 타겟에서 해제한다.
	- 만약 해당 지점에 도착하기 이전에 다른 지점을 클릭한 경우에는 타겟을 삭제하고, 위를 반복한다.







*/

void AGaren::MouseRightClick()
{
	UE_LOG(LogTemp,Warning, TEXT("Success"));

}
