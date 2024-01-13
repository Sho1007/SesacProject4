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

	// 건축물 외형 Mesh
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);


	StateComponentBase_Building = CreateDefaultSubobject<UStateComponentBase>(TEXT("StateComponentBase_Building"));

}

// Called when the game starts or when spawned
void ABuilding_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuilding_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 인터페이스 함수 - 자식 클래스에서 각자 재정의
void ABuilding_Base::Damaged() {}
void ABuilding_Base::Die() {}

