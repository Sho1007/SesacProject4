// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Nexus.h"
#include <Components/CapsuleComponent.h>
#include "Component/StateComponentBase.h"

ANexus::ANexus()
{
	// 넥서스의 Collision세팅
	CollisionComp->SetCapsuleRadius(350);


}

void ANexus::BeginPlay()
{
	Super::BeginPlay();

	NexusState = ENexusState::IDLE;
}

ENexusState  ANexus::GetNexusState() const
{
	return ENexusState();
}

void ANexus::TakeDamage_Nexus()
{
	StateComponent_Building->ApplyDamage(10.f); // 임시 10 - 타격 대상의 공격력이 들어가야 할 듯

	// 실행 순서
	// TakeDamage_Turret() -> StateComponent_Building->ApplyDamage() -> OnRep_Health() -> Damaged() / Die()

}

void ANexus::Damaged()
{
	// 넥서스의 체력이 0보다 크면 - 호출할 거 없을 듯?

}

void ANexus::Die()
{
	// 넥서스의 체력이 0 이하이면

	// 모든 콜리전 NoCollision - 넥서스는 콜리전 끌 필요 없을 듯?
	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 넥서스가 파괴되었을 때의 연출 필요




	// 넥서스 상태를 Destroy로 변경
	NexusState = ENexusState::Destroy;

}
