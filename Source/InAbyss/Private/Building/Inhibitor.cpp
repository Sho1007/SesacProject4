// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Inhibitor.h"
#include "Components/CapsuleComponent.h"
#include "Component/StateComponentBase.h"

AInhibitor::AInhibitor()
{
	// �������� Collision����
	CollisionComp->SetCapsuleRadius(210);

}

void AInhibitor::BeginPlay()
{
	Super::BeginPlay();

	InhibitorState = EInhibitorState::IDLE;
}

EInhibitorState AInhibitor::GetInhibitorState() const
{
	return EInhibitorState();
}

void AInhibitor::TakeDamage_Inhibitor()
{
	StateComponent_Building->ApplyDamage(10.f); // �ӽ� 10 - Ÿ�� ����� ���ݷ��� ���� �� ��

	// ���� ����
	// TakeDamage_Turret() -> StateComponent_Building->ApplyDamage() -> OnRep_Health() -> Damaged() / Die()

}

void AInhibitor::ReBuildTimer()
{
	// �������� ü���� 0�� �Ǿ��� �� ȣ��� �Լ�
	FTimerHandle RebuildTimerhandle;

	// 5���� Ÿ�̸Ӹ� �����ϰ�, Ÿ�̸Ӱ� ����Ǹ� �������� ü���� �ٽ� �ִ�� �����. 
	GetWorldTimerManager().SetTimer(RebuildTimerhandle, this, &AInhibitor::ReBuild, 300, false); 

}

void AInhibitor::ReBuild()
{
	InhibitorState = EInhibitorState::IDLE;

	// ������ ü���� Ǯ�� ä����
	//CurrentHealthPoiont_Building = MaxHealthPoiont_Building;

	// ������ ��Ȱ �ִϸ��̼�



}

void AInhibitor::Damaged()
{
	// �������� ü���� 0���� ũ�� - ȣ���� �� ���� ��?

}

void AInhibitor::Die()
{
	// �������� ü���� 0 �����̸�

	// ��� �ݸ��� NoCollision - ������� �ݸ��� �� �ʿ� ������?
	
	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �����Ⱑ �ı��Ǿ��� ���� ���� �ʿ� - �ִϸ��̼�




	// ������ ���¸� Destroy�� ����
	InhibitorState = EInhibitorState::Destroy;



	// ������ ��� Ÿ�̸� �Լ� ȣ��
	ReBuildTimer();

}

/*
������� �ı��� �� 5�� �Ŀ� ������ȴ�.
	-> ������� ü���� 0�� �Ǹ� Ÿ�̸Ӹ� �����Ѵ�.
	-> Ÿ�̸Ӱ� ������ �������� ü���� ȸ���Ѵ�.

�ؼ����� ������ ��ž�� ü�� ������ 3��еǾ��µ�, ��ž�� ü���� ���� ü���� ���� 3��� ������ �ִ�ġ(100%, 66%, 33%)������ ����ȴ�. ���� ��� 70%�� ���̸� 100%����, 50%�� ���̸� 66%����, 25%�� ���̸� 33%������ ȸ���ȴ�.

ü�� ��� 5�ʴ� 15

*/