// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Turret.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BD_Turret_Base.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystemComponent.h>

// Sets default values
AProjectile_Turret::AProjectile_Turret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);

	// ���߿��� Mesh�� ���� ����Ʈ�� ���ܵ� �� ��
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// �������Ʈ���� ����Ʈ ���� �ʿ�
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);




}

// Called when the game starts or when spawned
void AProjectile_Turret::BeginPlay()
{
	Super::BeginPlay();

	// ������ ������ ������ �������� ���󰡱� ������, ���۽� �������� �������־�� ��
	CollisionComp->SetRelativeScale3D(FVector(1));



}

// Called every frame
void AProjectile_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// �߻�ü�� AttackTarget�� ���� - Ȯ����������
	if (AttackTarget) {
		
		FVector Direction = (AttackTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		//SetActorRotation(Direction.Rotation());

		FVector TargetLocation = GetActorLocation() + Direction * Speed * DeltaTime;
		SetActorLocation(TargetLocation);

		//UE_LOG(LogTemp, Warning, TEXT("Success Tick"));

	}

	if (!AttackTarget) {
		Destroy();
	}

}

void AProjectile_Turret::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// �浹�� ���Ͱ� AttackTarget�� ��쿡�� �Լ��� ȣ�� -  �̿��� ���͵��� ������ �� �ְ� ��
	if (OtherActor == AttackTarget) {
		
		Attack_Overlap();

	}
}

void AProjectile_Turret::OwnerSetting()
{
	// �ڽ��� Owner�� ����� �� �ֵ��� �����ͼ� ������ ������
	OwnerTurret = Cast<ABD_Turret_Base>(GetOwner());

	//UE_LOG(LogTemp, Warning, TEXT("this : %s"), *this->GetOwner()->GetName());

}

void AProjectile_Turret::AttackTargeting()
{
	if (AttackTarget) {
		return;
	}

	if (OwnerTurret) {
		// �߻�ü�� ���� Ÿ���� ������
		if (OwnerTurret->TopPriorityTarget) {

			// �ֿ켱 ������ Ÿ���� �ִٸ� �̸� AttackTarget�� ����
			AttackTarget = OwnerTurret->TopPriorityTarget;

			//UE_LOG(LogTemp, Warning, TEXT("AttackTarget : %s"), *AttackTarget->GetName());

		}
		else if (OwnerTurret->CurrentTarget) {

			// ����� Ÿ���� �ִٸ� �̸� AttackTarget�� ����
			AttackTarget = OwnerTurret->CurrentTarget;

			//UE_LOG(LogTemp, Warning, TEXT("AttackTarget : %s"), *AttackTarget->GetName());

		}
		else {
			// Ÿ���� ���ٸ� �߻�ü�� �ı�
			Destroy();
		}

	}
}

void AProjectile_Turret::Attack_Overlap()
{
	
	// AttackTarget���� �������� �־�� ��


	// �ڽ��� �ı�
	Destroy();

}

/*

�߻�ü�� AttackTarget�� ���󰡵��� �ؾ� ��

AttackTarget �̿ܿ��� �ٸ� ���Ϳʹ� �浹�ϴ��� �����ؾ� ��

�߻�ü�� AttackTarget�� �浹�� ���, AttackTarget���� �������� �־�� ��

*/