// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Nexus.h"
#include <Components/CapsuleComponent.h>
#include "Component/StateComponentBase.h"

ANexus::ANexus()
{
	// �ؼ����� Collision����
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
	StateComponent_Building->ApplyDamage(10.f); // �ӽ� 10 - Ÿ�� ����� ���ݷ��� ���� �� ��

	// ���� ����
	// TakeDamage_Turret() -> StateComponent_Building->ApplyDamage() -> OnRep_Health() -> Damaged() / Die()

}

void ANexus::Damaged()
{
	// �ؼ����� ü���� 0���� ũ�� - ȣ���� �� ���� ��?

}

void ANexus::Die()
{
	// �ؼ����� ü���� 0 �����̸�

	// ��� �ݸ��� NoCollision - �ؼ����� �ݸ��� �� �ʿ� ���� ��?
	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �ؼ����� �ı��Ǿ��� ���� ���� �ʿ�




	// �ؼ��� ���¸� Destroy�� ����
	NexusState = ENexusState::Destroy;

}
