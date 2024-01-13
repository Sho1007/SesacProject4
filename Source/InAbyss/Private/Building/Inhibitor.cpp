// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Inhibitor.h"
#include "Components/CapsuleComponent.h"

AInhibitor::AInhibitor()
{
	// �������� Collision����
	CollisionComp->SetCapsuleRadius(210);

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
	CurrentHealthPoiont_Building = MaxHealthPoiont_Building;

}

/*
������� �ı��� �� 5�� �Ŀ� ������ȴ�.
	-> ������� ü���� 0�� �Ǹ� Ÿ�̸Ӹ� �����Ѵ�.
	-> Ÿ�̸Ӱ� ������ �������� ü���� ȸ���Ѵ�.

�ؼ����� ������ ��ž�� ü�� ������ 3��еǾ��µ�, ��ž�� ü���� ���� ü���� ���� 3��� ������ �ִ�ġ(100%, 66%, 33%)������ ����ȴ�. ���� ��� 70%�� ���̸� 100%����, 50%�� ���̸� 66%����, 25%�� ���̸� 33%������ ȸ���ȴ�.

ü�� ��� 5�ʴ� 15

*/