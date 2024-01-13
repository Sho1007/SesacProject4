// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Projectile_Turret.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Building/Turret_Base.h"

// Sets default values
AProjectile_Turret::AProjectile_Turret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);

	// 나중에는 Mesh는 빼고 이펙트만 남겨도 될 듯
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// 블루프린트에서 이펙트 설정 필요
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProjectile_Turret::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectile_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 발사체가 AttackTarget를 따라감 - 확정유도공격
	if (AttackTarget) {

		FVector Direction = (AttackTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		SetActorRotation(Direction.Rotation());

		FVector TargetLocation = GetActorLocation() + Direction * Speed * DeltaTime;
		SetActorLocation(TargetLocation);

		//UE_LOG(LogTemp, Warning, TEXT("Success Tick"));

	}
	else {
		Destroy();

	}

}

void AProjectile_Turret::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// 충돌한 액터가 AttackTarget인 경우에만 함수를 호출 -  이외의 액터들은 무시할 수 있게 됨
	if (OtherActor == AttackTarget) {

		Attack_OverlapTarget();

	}
}

void AProjectile_Turret::OwnerSetting()
{
	// 자신의 Owner를 사용할 수 있도록 가져와서 변수에 저장함
	OwnerTurret = Cast<ATurret_Base>(GetOwner());

	//UE_LOG(LogTemp, Warning, TEXT("this : %s"), *this->GetOwner()->GetName());

}

void AProjectile_Turret::AttackTargeting()
{
	// 이미 공격 대상이 정해져있다면 함수 종료
	if (AttackTarget) {
		return;
	}
	
	if (OwnerTurret) {
		// 발사체가 따라갈 타겟을 저장함
		if (OwnerTurret->TopPriorityTarget) {

			// 최우선 순위의 타겟이 있다면 이를 AttackTarget에 저장
			AttackTarget = OwnerTurret->TopPriorityTarget;

			//UE_LOG(LogTemp, Warning, TEXT("AttackTarget : %s"), *AttackTarget->GetName());

		}
		else if (OwnerTurret->CurrentTarget) {

			// CurrentTarget이 있다면 이를 AttackTarget에 저장
			AttackTarget = OwnerTurret->CurrentTarget;

			//UE_LOG(LogTemp, Warning, TEXT("AttackTarget : %s"), *AttackTarget->GetName());

		}
		else {
			// 타겟이 없다면 발사체를 파괴
			Destroy();
		}

	}

}

void AProjectile_Turret::Attack_OverlapTarget()
{
	// AttackTarget에게 데미지를 주어야 함


	// 자신을 파괴
	Destroy();

}

/*

발사체가 AttackTarget를 따라가도록 해야 함

AttackTarget 이외에는 다른 액터와는 충돌하더라도 무시해야 함

발사체가 AttackTarget과 충돌한 경우, AttackTarget에게 데미지를 주어야 함

*/
