// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Nexus.h"
#include <Components/CapsuleComponent.h>
#include "Component/StateComponentBase.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ANexus::ANexus()
{
	// �ؼ����� Collision����
	CollisionComp->SetCapsuleRadius(350);


}

void ANexus::BeginPlay()
{
	Super::BeginPlay();

	BuildingState = EBuildingState::IDLE;

	ScanBuildingInLevel();

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
	BuildingState = EBuildingState::Destroy;

}
