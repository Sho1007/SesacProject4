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

	// 건축물 콜리전
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetCapsuleHalfHeight(500);
	CollisionComp->SetCapsuleRadius(130);

	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);



	// 건축물 외형 Mesh
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);


	StateComponent_Building = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComponentBase_Building"));

}

// Called when the game starts or when spawned
void ABuilding_Base::BeginPlay()
{
	Super::BeginPlay();

	// 만약 내 앞의 건물이 남아 있다면, 무적상태가 되어야 함 - 콜리전을 꺼버림
	if (PreBuilding.Num() != 0) {
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}

	StateComponent_Building->SetObjectType(EObjectType::BUILDING);


}

// Called every frame
void ABuilding_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	if (PreBuilding.Num() != 0) {
		
		for (ABuilding_Base* Building : PreBuilding) {
			
			if (Building->BuildingState != EBuildingState::Destroy) {

				CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				return;
			}
			
		}

		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}
	*/


}

EBuildingState ABuilding_Base::GetBuildingState() const
{
	return EBuildingState();
}

// 인터페이스 함수 - 자식 클래스에서 각자 재정의
void ABuilding_Base::Damaged() {}
void ABuilding_Base::Die() {}

void ABuilding_Base::TakeDamage_Building()
{
	StateComponent_Building->ApplyDamage(10.f); // 임시 10 - 타격 대상의 공격력이 들어가야 할 듯

	// 실행 순서
		// TakeDamage_Turret() -> StateComponent_Building->ApplyDamage() -> OnRep_Health() -> Damaged() / Die()
}

void ABuilding_Base::ActivateHit()
{
	// 이거 안 씀

	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//UE_LOG(LogTemp, Warning, TEXT("You can2"));
}
