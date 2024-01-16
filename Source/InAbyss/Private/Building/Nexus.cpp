// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Nexus.h"
#include <Components/CapsuleComponent.h>
#include "Component/StateComponentBase.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "AnimInstance/NexusAnimInstance.h"

ANexus::ANexus()
{
	// �ؼ����� Collision����
	CollisionComp->SetCapsuleRadius(350);

	/*
	NexusAnimInstance = Cast<UNexusAnimInstance>(MeshComp->GetAnimInstance());
	check(NexusAnimInstance);
	*/
}

void ANexus::BeginPlay()
{
	Super::BeginPlay();

	BuildingState = EBuildingState::IDLE;

	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//ScanBuildingInLevel();

}

void ANexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BuildingState != EBuildingState::Destroy && PreBuilding.Num() != 0) {

		for (ABuilding_Base* Building : PreBuilding) {

			if (Building->BuildingState != EBuildingState::Destroy) {

				CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				return;
			}

		}

		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}

}

void ANexus::ScanBuildingInLevel()
{
	// �� ��� �ʿ� ����

	// ������ Building Ŭ������ ���͸� ��� �迭�� �����ϴ� ���
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuilding_Base::StaticClass(), AllBuilding);

	FVector NexusLocation = GetActorLocation();

	// AllBuilding �迭�� �ؼ������� ����� ������ �����ϴ� ���
	AllBuilding.Sort([NexusLocation](const AActor& A, const AActor& B) {
		float DistA = FVector::Dist(NexusLocation, A.GetActorLocation());
		float DistB = FVector::Dist(NexusLocation, B.GetActorLocation());
		return DistA < DistB;

		});

	AllBuilding.Num();

	//UE_LOG(LogTemp, Warning, TEXT("Building : %d"), AllBuilding.Num());

	for (int i = 1; i < AllBuilding.Num(); ++i)
	{
		for (int j = i + 1; j < AllBuilding.Num(); ++j)
		{
			Cast<ABuilding_Base>(AllBuilding[i])->PreBuilding.Add(Cast<ABuilding_Base>(AllBuilding[j]));
		}
	}


	
	
	/*for (int i = 0; i < AllBuilding.Num(); i++) {

		if (i == 0) {
			Cast<ABuilding_Base>(AllBuilding[0])->PreBuilding.Add(Cast<ABuilding_Base>(AllBuilding[1]));
			Cast<ABuilding_Base>(AllBuilding[0])->PreBuilding.Add(Cast<ABuilding_Base>(AllBuilding[2]));

		}
	


		else if (i == 1) {
			Cast<ABuilding_Base>(AllBuilding[1])->PreBuilding.Add(Cast<ABuilding_Base>(AllBuilding[3]));

		}
		else if (i == 2) {
			Cast<ABuilding_Base>(AllBuilding[2])->PreBuilding.Add(Cast<ABuilding_Base>(AllBuilding[3]));

		}


		else if (i == 3) {
			Cast<ABuilding_Base>(AllBuilding[3])->PreBuilding.Add(Cast<ABuilding_Base>(AllBuilding[4]));

		}
		else if (i == 4) {
			Cast<ABuilding_Base>(AllBuilding[4])->PreBuilding.Add(Cast<ABuilding_Base>(AllBuilding[5]));

		}
		else if (i == 5) {
			Cast<ABuilding_Base>(AllBuilding[5])->PreBuilding.Add(Cast<ABuilding_Base>(AllBuilding[6]));

		}
		else if (i == 6) {
			Cast<ABuilding_Base>(AllBuilding[6])->PreBuilding.Reset();

		}


		else {
			return;
		}

	}*/



}

void ANexus::Damaged()
{
	// �ؼ����� ü���� 0���� ũ�� - ȣ���� �� ���� ��?

}

void ANexus::Die()
{
	// �ؼ����� ü���� 0 �����̸�

	// ��� �ݸ��� NoCollision - �ؼ����� �ݸ��� �� �ʿ� ���� ��?
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*
	// �ؼ����� �ı��Ǿ��� ���� ���� �ʿ�
	NexusAnimInstance->bIsDeath = true;
	*/


	// �ؼ��� ���¸� Destroy�� ����
	BuildingState = EBuildingState::Destroy;

}
