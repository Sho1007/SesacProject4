// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_Turret_Base.h"
#include "Components/SphereComponent.h"
#include "Building_Base.h"
#include "Components/SceneComponent.h"
#include "Projectile_Turret.h"

ABD_Turret_Base::ABD_Turret_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	// ��ž�� �����Ÿ��� ������ �ݸ���
	DetectCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCollisionComp"));
	DetectCollisionComp->SetupAttachment(RootComponent);
	DetectCollisionComp->SetRelativeScale3D(FVector(4.0f));
	DetectCollisionComp->SetRelativeLocation(FVector(0, 0, -20));

	// ���� - �߻�ü�� ������ ��ġ
	AttackStartPointComp = CreateDefaultSubobject<USceneComponent>(TEXT("AttackStartPointComp"));
	AttackStartPointComp->SetupAttachment(RootComponent);
	AttackStartPointComp->SetRelativeLocation(FVector(0, 0, 90));

	UE_LOG(LogTemp, Warning, TEXT("New Log========================================================"));

}

void ABD_Turret_Base::Tick(float DeltaTime)
{

	// �浹 ���� ���Ͱ� �ְ�, CurrentTarget�� ���� ���
	if (DetectTargets_Test.Num() > 0 && CurrentTarget == nullptr) {
		
		// ���ݴ�� ������ �Լ� ȣ��
		RetargetCurrentTarget();

	}

	// ----------------------------------------

	// ���ݱ��
	// ���� ���ݴ���� �����Ǿ� �ִٸ�
	if (CurrentTarget || TopPriorityTarget) {
		
		// �ð��� ī��Ʈ ����
		CurrentTIme += DeltaTime;

		// 2�ʰ� ������
		if (CurrentTIme >= TargetTIme) {

			CurrentTIme = 0.f;

			// �����Լ� ȣ�� - �߻�ü ����
			Attact_SpawnProjectile();
		}

	}
	else
	{
		CurrentTIme = 0.f;
	}
	
	if (TopPriorityTarget) {
		// ����׶��� ���� ����Ʈ�� CurrentTarget�� ����Ű���� �ؾ� ��
		DrawDebugLine(GetWorld(), AttackStartPointComp->GetComponentLocation(), TopPriorityTarget->GetActorLocation(), FColor::Red, false, -1, 0, 1.0f);
	}
	else if (CurrentTarget) {
		// ����׶��� ���� ����Ʈ�� CurrentTarget�� ����Ű���� �ؾ� ��
		DrawDebugLine(GetWorld(), AttackStartPointComp->GetComponentLocation(), CurrentTarget->GetActorLocation(), FColor::Red, false, -1, 0, 1.0f);
	}

}

void ABD_Turret_Base::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// test ==================================================================================================

	// ��ž�� ������ ���͸� �迭�� ���� -> �浹�� ���͸� �迭�� ���� 
	// + ������ ���Ͱ� ���� ��쿡�� �����ϵ��� �ؾ� ��*************************
	DetectTargets_Test.Add(OtherActor);

	/* &&&&&&&&&&&&&&&&&�α�
	// �迭�� �ε��� ���� ����ϴ� �α�
	for (AActor* Actor : DetectTargets_Test)
	{
		if (Actor)
		{
			// AActor* Ÿ���� �ּҰ��� ����ϰų�, �ش� ������ �̸� ���� ����� �� �ֽ��ϴ�.
			UE_LOG(LogTemp, Warning, TEXT("Detected Actor: %s"), *Actor->GetName());
		}
	}
	*/

	// CurrentTarget�� ���� ���, �迭�� 0�� �ε��� = ���� ���� ������ ���͸� CurrentTarget�� ���� -> ���ݴ������ ����
	if (CurrentTarget == nullptr) {

		CurrentTarget = DetectTargets_Test[0];




		UE_LOG(LogTemp, Warning, TEXT("CurrentTarget : %s"), *CurrentTarget->GetName());

	}

	// test ==================================================================================================

	// ��ž�� �ݸ����� �浹�ϴ� ���͸� �迭�� �����Ѵ�. 
	/*

	if (OtherActor->IsA<Ŭ���� �̸�>()) {
		DetectTargets_SuperOrCanon.Add(OtherActor);

	}
	else if (OtherActor->IsA<Ŭ���� �̸�>()) {
		DetectTargets_Warrior.Add(OtherActor);

	}
	else if (OtherActor->IsA<Ŭ���� �̸�>()) {
		DetectTargets_Wizard.Add(OtherActor);

	}
	else if (OtherActor->IsA<Ŭ���� �̸�>()) {
		DetectTargets_EnemyChampion.Add(OtherActor);

	}



	// ���߿� ���� �ʿ� - OtherActor�� è�Ǿ� Ȥ�� �̴Ͼ��� ��쿡�� ���� => OtherActor�� if������ Ŭ������ �˻��� ��, Ŭ������ �´� �迭�� ������ ����
	DetectTargets_SuperOrCanon.Add(OtherActor);

	DetectTargets_Warrior.Add(OtherActor);

	DetectTargets_Wizard.Add(OtherActor);

	DetectTargets_EnemyChampion.Add(OtherActor);
	*/


	//UE_LOG(LogTemp, Warning, TEXT("BiginOverlap-----------------------------------------------------"));

}

void ABD_Turret_Base::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	// => ��Ż�� OtherActor�� Ŭ������ �˻��� ��, Ŭ������ �ش��ϴ� �迭���� �ش� ���͸� �����ؾ� ��

	// test ==================================================================================================

	// ��ž�� �������� ��� ���ʹ� �迭���� ���� -> �浹�� ������ ���͸� �迭���� �����Ѵ�. 
	DetectTargets_Test.Remove(OtherActor); // ��ǻ� ������δ� �� �ڵ常 �ʿ���

	/*
	// ��ž�� ���� ���� ���Ͱ� �����ִ��� �˻��ϴ� �α� 
	if (DetectTargets_Test.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("No Detect"));

	}
	else {
		for (AActor* Actor : DetectTargets_Test)
		{
			if (Actor)
			{
				// AActor* Ÿ���� �ּҰ��� ����ϰų�, �ش� ������ �̸� ���� ����� �� �ֽ��ϴ�.
				UE_LOG(LogTemp, Warning, TEXT("Detected Actor: %s"), *Actor->GetName());
			}
		}
	}
	*/
	// test ==================================================================================================










	// ����Ÿ���� ��Ż�� ��� - CurrentTarget�� è�Ǿ��̰�, ��ž�� �������� ��� ���
	if (CurrentTarget && CurrentTarget == OtherActor) {

		CurrentTarget = nullptr;




		//UE_LOG(LogTemp, Warning, TEXT("No CurrentTarget"));

		/*
		// �� �κ��� ��� �� ��
		// ���� Ÿ���� �������� ��, DetectTargets_Test�� ũ�Ⱑ 1�̻� �̶�� �켱������ ���� CurrentTarget�� �缳���ϴ� �Լ��� ȣ���Ѵ�.
		if (DetectTargets_Test.Num() > 0) {

			RetargetCurrentTarget();

		}
		*/
	}

	//UE_LOG(LogTemp, Warning, TEXT("EndOverlap-----------------------------------------------------"));

}

void ABD_Turret_Base::RetargetCurrentTarget()
{
	// �浹�ϰ� �ִ� ������ �켱������ ���� CurrentTarget�� �������Ѵ�. 

	// CurrentTarget�� �ִٸ� �Լ� ����
	if (CurrentTarget) {
		return;
	}



	// test ==================================================================================================
	// �����ִ� �迭�� �ִٸ�, 0�� �ε����� ���� ������� �� 
	if (DetectTargets_Test.Num() > 0) {
		CurrentTarget = DetectTargets_Test[0];



		/*
		// Ÿ���� ����ϴ� �α�
		if (CurrentTarget) {
			UE_LOG(LogTemp, Warning, TEXT("CurrentTarget : %s"), *CurrentTarget->GetName());

		}
		else if (CurrentTarget == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("No CurrentTarget"));

		}
		*/
	}

	// test ==================================================================================================





	// 1. ����/���� �̴Ͼ� ==== 2.���� �̴Ͼ� ==== 3. ������ �̴Ͼ� ==== 4. �� è�Ǿ�
	if (DetectTargets_SuperOrCanon.Num() > 0) {
		CurrentTarget = DetectTargets_SuperOrCanon[0];
	}
	else if (DetectTargets_Warrior.Num() > 0) {
		CurrentTarget = DetectTargets_Warrior[0];

	}
	else if (DetectTargets_Wizard.Num() > 0) {
		CurrentTarget = DetectTargets_Wizard[0];
	}
	else if (DetectTargets_EnemyChampion.Num() > 0) {
		CurrentTarget = DetectTargets_EnemyChampion[0];

	}

	UE_LOG(LogTemp, Warning, TEXT("CurrentTarget : %s"), *CurrentTarget->GetName());


}

void ABD_Turret_Base::Attact_SpawnProjectile()
{
	// �̰Ŵ� �ƴ� ��
	/*
	FHitResult hitInfo;
	FVector StartLocation = AttackStartPointComp->GetComponentLocation();
	FVector EndLocation = CurrentTarget->GetActorLocation();

	GetWorld()->LineTraceSingleByChannel(hitInfo, StartLocation, EndLocation, ECC_Visibility);
	*/
	
	
	// �߻�ü�� �����ؾ� �� - ���� ��ü�� �߻�ü�� Ÿ���� ���󰡴� ������� �̷���� ��
	AProjectile_Turret* NewProjectile = GetWorld()->SpawnActor<AProjectile_Turret>(ProjectileFactory, AttackStartPointComp->GetComponentTransform());
	
	//UE_LOG(LogTemp, Warning, TEXT("Spawn Attack"));

	if (NewProjectile) {
		NewProjectile->SetOwner(this);

		//UE_LOG(LogTemp, Warning, TEXT("NewProjectile : %s"), *NewProjectile->GetName());

		NewProjectile->OwnerSetting();

		NewProjectile->AttackTargeting();

	}

}



/*

����

���� TopPriorityTarget�� ����� ���� �ִٸ�, �ش� ���͸� �����Ѵ�.

�׷��� �ʰ�, CurrentTarget�� ���� �ִٸ� �ش� ���͸� �����Ѵ�.

���� �� �� ������ ���ٸ�, �ƹ��� �������� �ʴ´�.



������ �ݵ�� �����ϰ�, ��ǥ�� �Ѿư���.

���� ������ �αٿ��� ���ڷ� ������ �׷����鼭 Ÿ������ �ǰ�,
������ �ð�(2��)���� �߻�ü�� �߻��Ѵ�.

�߻�� �߻�ü�� ��ǥ�� �ѾƼ� ���ư���,
�߻�ü�� ��ǥ�� �浹�ϸ� �������� ������.

*/

/*

�÷��̾� �� �̴Ͼ��� ����

��������
��ž�� �ݸ����� �浹 ���� ���͸� �����Ѵ�.

1�� - �ݸ����� ���ο� ���Ͱ� �浹�� ������, �ش� ���͸� DetectTargets �迭�� �����Ѵ�.
	1.1�� - �浹�� ������� �迭�� �����Ѵ�.
		->

2�� - �ݸ������� ��Ż�� ������ �ִٸ�, �ش� ������ DetectTargets �迭���� �����Ѵ�.
		->


*/

/*

��������
��ž�� ���� ������ ���ʹ� ���ݽÿ� ��ž�� �ݸ����� �浹 ���� ���ͻ��̴�.
		-> DetectTargets �迭�� ũ�Ⱑ 1 �̻��� ��쿡 ȣ���� �� �ִ�.

Ư������(�ֿ켱����)
0�� - 1��~3�� �� ��� ����̴���, �ݸ����� �浹���� �� è�Ǿ��� �Ʊ��� è�Ǿ��� ������ ���, �ش� �� è�Ǿ��� ���� ������� �Ѵ�.
		-> �̹� CurrentTarget�� ����� ���� �ִ���, �Ʊ� è�Ǿ� �ǰݽ� ȣ��ȴ�.
		-> ��� ����/�ǰ� ���θ� ����????? -> ������ Ȥ�� �������� è�Ǿ��� �˷��� �� => ������ ���� ���� ��
		-> �Ʊ� è�Ǿ��� ������ �� è�Ǿ��� TopPriorityTarget�� �����Ѵ�.




�Ϲ�����(�켱����)
1�� - ���� ���� ������ ������ ���� ������� ���, �����Ѵ�.
		-> CurrentTarget�� ����ִ� ���, DetectTargets�� 0�� �ε����� ������ CurrentTarget�� �����Ѵ�.

2�� - CurrentTarget�� ����� ������ ��� Ȥ�� ��Ż�� ���, ���ο� ���� ����� ã�´�.
		-> ��ž�� ������ ������ ��, CurrentTarget�� ���� ���ٸ� 3���� ȣ���Ѵ�.

3�� - ���� �� �� �̹� �ݸ����� �浹�ϰ� �ִ� ���Ͱ� �ִٸ�, �� �ȿ��� �켱������ ���ǿ� ���� ���� ����� �����Ѵ�.
	3.1�� - ����/���� �̴Ͼ� -> ���� �̴Ͼ� -> ������ �̴Ͼ� -> �� è�Ǿ�
	3.2�� ���� �켱�������, ���� ������ ������ ���� ������� �Ѵ�.
		-> CurrentTarget�� ���� ����, DetectTargets�� ũ�Ⱑ 1�̻��� ��쿡 ȣ���� �� �ִ�.
		-> if���� ���� �켱������ �ش��ϴ� ������ ã�´�.
		-> �迭�� ����ִ� �ε����� 0������ �˻��Ѵ�.
		-> ���� �˻��ϴ� �ε����� ������ CurrentTarget�� ����� ���ֺ��� �켱������ ���ٸ�, �ش� �ε����� ������ CurrentTarget�� �����Ѵ�.


*/


