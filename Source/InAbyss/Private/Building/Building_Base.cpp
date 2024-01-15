// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Building_Base.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Component/StateComponentBase.h"

// Sets default values
ABuilding_Base::ABuilding_Base()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���๰ �ݸ���
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetCapsuleHalfHeight(500);
	CollisionComp->SetCapsuleRadius(130);

	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);



	// ���๰ ���� Mesh
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);


	StateComponent_Building = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComponentBase_Building"));

}

// Called when the game starts or when spawned
void ABuilding_Base::BeginPlay()
{
	Super::BeginPlay();

	if (PreBuilding.Num() == 0) {
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}
}

// Called every frame
void ABuilding_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	if (CollisionComp->GetCollisionEnabled() != ECollisionEnabled::NoCollision) {

		for (ABuilding_Base* Building : PreBuilding) {
	
			if (Building->BuildingState != EBuildingState::Destroy) {
			
				UE_LOG(LogTemp, Warning, TEXT("You can't1"));


				break;
			}

			UE_LOG(LogTemp, Warning, TEXT("3"));

		}

	}
	*/
	


	if (CollisionComp->GetCollisionEnabled() != ECollisionEnabled::QueryOnly) {
		ActivateHit();

	}
}

EBuildingState ABuilding_Base::GetBuildingState() const
{
	return EBuildingState();
}

// �������̽� �Լ� - �ڽ� Ŭ�������� ���� ������
void ABuilding_Base::Damaged() {}
void ABuilding_Base::Die() {}

void ABuilding_Base::ActivateHit()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//UE_LOG(LogTemp, Warning, TEXT("You can2"));
}
