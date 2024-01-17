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
�̵� - ���콺�� Ŭ���� ������ ����Ʈ���̽��� ���.
������ �ε��� ������ ��ġ ���� �����ͼ� ĳ���Ͱ� �̵��ϵ��� �Ѵ�.
���� �ε��� ������ �̴Ͼ�, Ÿ���� �ִٸ�, �ش� ���Ͱ� �ִٸ�, �̸� ���� Ÿ������ �����ϰ� �� ��ҷ� �̵��Ѵ�.
	- �ش� ������ Destroy Ȥ�� ����� ��쿡�� Ÿ�ٿ��� �����Ѵ�.
	- ���� �ش� ������ �����ϱ� ������ �ٸ� ������ Ŭ���� ��쿡�� Ÿ���� �����ϰ�, ���� �ݺ��Ѵ�.







*/

void AGaren::MouseRightClick()
{
	UE_LOG(LogTemp,Warning, TEXT("Success"));

}
