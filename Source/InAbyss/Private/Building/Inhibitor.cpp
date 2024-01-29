// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Inhibitor.h"
#include "Components/CapsuleComponent.h"
#include "Component/StateComponentBase.h"
#include "AnimInstance/InhibitorAnimInstance.h"

AInhibitor::AInhibitor()
{
	// 억제기의 Collision세팅
	CollisionComp->SetCapsuleRadius(210);

}

void AInhibitor::BeginPlay()
{
	Super::BeginPlay();

	BuildingState = EBuildingState::IDLE;

	// InhibitorAnimInstance = Cast<UInhibitorAnimInstance>(MeshComp->GetAnimInstance());
	// check(InhibitorAnimInstance);

}

void AInhibitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BuildingState != EBuildingState::Destroy && PreBuilding.Num() != 0) {

		for (ABuilding_Base* Building : PreBuilding) {

			if (Building->BuildingState != EBuildingState::Destroy) {

				return;
			}

		}

		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}


}

void AInhibitor::ReBuildTimer()
{
	// 억제기의 체력이 0이 되었을 때 호출될 함수
	FTimerHandle RebuildTimerhandle;

	// 5분의 타이머를 진행하고, 타이머가 종료되면 억제기의 체력을 다시 최대로 만든다. 
	GetWorldTimerManager().SetTimer(RebuildTimerhandle, this, &AInhibitor::ReBuild, 300, false); 

}

void AInhibitor::ReBuild()
{
	BuildingState = EBuildingState::IDLE;

	// 억제기 부활 애니메이션
	// 애니메이션 연출을 위한 변수
	// InhibitorAnimInstance->bIsDeath = false;

	// 억제기 체력을 풀로 채워줌
	StateComponent_Building->SetHealthToMax();
	

}

void AInhibitor::Damaged()
{
	// 억제기의 체력이 0보다 크면 - 호출할 거 없을 듯?

}

void AInhibitor::Die()
{
	// 억제기의 체력이 0 이하이면

	// 모든 콜리전 NoCollision - 억제기는 콜리전 끌 필요 없을듯?
	
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 억제기가 파괴되었을 때의 연출 필요 - 애니메이션


	// InhibitorAnimInstance->bIsDeath = true;


	// 억제기 상태를 Destroy로 변경
	BuildingState = EBuildingState::Destroy;



	// 억제기 재생 타이머 함수 호출
	ReBuildTimer();

}

/*
억제기는 파괴된 후 5분 후에 재생성된다.
	-> 억제기는 체력이 0이 되면 타이머를 시작한다.
	-> 타이머가 끝나면 억제기의 체력을 회복한다.

넥서스와 억제기 포탑의 체력 구간이 3등분되었는데, 포탑의 체력은 현재 체력이 속한 3등분 구간의 최대치(100%, 66%, 33%)까지만 재생된다. 예를 들어 70%로 깎이면 100%까지, 50%로 깎이면 66%까지, 25%로 깎이면 33%까지만 회복된다.

체력 재생 5초당 15

*/