// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Turret_Base.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Building/Projectile_Turret.h"

ATurret_Base::ATurret_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	// ��ž�� �����Ÿ��� ������ �ݸ���
	DetectCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCollisionComp"));
	DetectCollisionComp->SetupAttachment(RootComponent);
	DetectCollisionComp->SetSphereRadius(500);

	// ���� - �߻�ü�� ������ ��ġ
	AttackStartPointComp = CreateDefaultSubobject<USceneComponent>(TEXT("AttackStartPointComp"));
	AttackStartPointComp->SetupAttachment(RootComponent);
	AttackStartPointComp->SetRelativeLocation(FVector(50, 160, 590));

	UE_LOG(LogTemp, Warning, TEXT("New Play Log========================================================"));

}

void ATurret_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CurrentTarget�� ����, �浹 ���� ���Ͱ� �ִ� ���
	if (CurrentTarget == nullptr && DetectTargets_Test.Num() > 0 ) { //  ���� ���� �̴Ͼ�, è�Ǿ� �迭�� �߰��ؼ� ���� �ʿ�

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
	else // ���ݴ���� ������ �ð� �ʱ�ȭ
	{
		CurrentTIme = 0.f;
	}

	// ----------------------------------------

	// Ÿ���� ��� ǥ�� ���
	if (TopPriorityTarget) { // �ֿ켱����
		// ����׶��� ���� ����Ʈ�� CurrentTarget�� ����Ű���� �ؾ� ��
		DrawDebugLine(GetWorld(), AttackStartPointComp->GetComponentLocation(), TopPriorityTarget->GetActorLocation(), FColor::Red, false, -1, 0, 1.0f);
	}
	else if (CurrentTarget) { // �켱���� 
		// ����׶��� ���� ����Ʈ�� CurrentTarget�� ����Ű���� �ؾ� ��
		DrawDebugLine(GetWorld(), AttackStartPointComp->GetComponentLocation(), CurrentTarget->GetActorLocation(), FColor::Red, false, -1, 0, 1.0f);
	}

}

void ATurret_Base::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// test ==================================================================================================
	// ��ž�� ������ ���͸� �迭�� ���� -> �浹�� ���͸� �迭�� ���� 

	// �ӽ� : �߻�ü�� �迭�� �������� ���� - �̰� ���߿� �迭�� �̴Ͼ�, è�Ǿ� �ν��ϵ��� �ϸ� �ʿ� ����
	if (OtherActor->IsA<AProjectile_Turret>()) { // �ӽ�
		return;
	}




	// ������ ���� ����� ���ݴ�� �迭�� ����
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
	// + ������ ���Ͱ� ���� ��쿡�� �����ϵ��� �ؾ� ��*************************
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



	*/


	//UE_LOG(LogTemp, Warning, TEXT("BiginOverlap-----------------------------------------------------"));
}

void ATurret_Base::NotifyActorEndOverlap(AActor* OtherActor)
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







	// ��Ż ����� �ֿ켱 Ÿ���̶��, �̸� �������� ��
	if (TopPriorityTarget  && TopPriorityTarget == OtherActor) {
	
		TopPriorityTarget = nullptr;

	}

	// ����Ÿ���� ��Ż�� ��� - CurrentTarget�� ��ž�� �������� ��� ���
	if (CurrentTarget && CurrentTarget == OtherActor) {
		
		CurrentTarget = nullptr;

	}

	// ��ž�� �������� ��Ż�� ���͸� �迭���� �����Ѵ�. - ������ �� �ε����� �ڵ����� ��ܼ� ���ĵȴ�. 
	/*

	if (OtherActor->IsA<Ŭ���� �̸�>()) {
		DetectTargets_SuperOrCanon.Remove(OtherActor);

	}
	else if (OtherActor->IsA<Ŭ���� �̸�>()) {
		DetectTargets_Warrior.Remove(OtherActor);

	}
	else if (OtherActor->IsA<Ŭ���� �̸�>()) {
		DetectTargets_Wizard.Remove(OtherActor);

	}
	else if (OtherActor->IsA<Ŭ���� �̸�>()) {
		DetectTargets_EnemyChampion.Remove(OtherActor);

	}
	*/


}

void ATurret_Base::RetargetCurrentTarget()
{
	// ���� ���ݹ޴� Ÿ���� ������� ��� ȣ���ϵ��� �ؾ� �Ѵ�. - CurrentTarget�� nullprt�� ���
	// Ȥ�� �ֿ켱 ����� ���� ���, ȣ���ؾ� �Ѵ�. - if(TopPriorityTarget) �� ���
	
	// �浹�ϰ� �ִ� ������ �켱������ ���� CurrentTarget�� �������Ѵ�. 

	// ���� �ֿ켱���� ����� �ִٸ�, �̸� CurrentTarget�� �ϰ� �Լ� ����
	if (TopPriorityTarget) {
		CurrentTarget = TopPriorityTarget;

		return;
	}

	// ���ݴ���� �ִٸ� �Լ� ����
	if (CurrentTarget) {

		return;
	}



	// test ==================================================================================================
	// �����ִ� �迭�� �ִٸ�, 0�� �ε����� ���� ������� �� 
	if (DetectTargets_Test.Num() > 0) {
		CurrentTarget = DetectTargets_Test[0];

		return;

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



	// �����ִ� �迭�� �ִٸ�, �켱������ ���� 0�� �ε����� ���� ������� �� 
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

void ATurret_Base::Attact_SpawnProjectile()
{

	// �߻�ü�� �����ؾ� �� - ���� ��ü�� �߻�ü�� Ÿ���� ���󰡴� ������� �̷���� ��
	AProjectile_Turret* NewProjectile = GetWorld()->SpawnActor<AProjectile_Turret>(ProjectileFactory, AttackStartPointComp->GetComponentTransform());

	//UE_LOG(LogTemp, Warning, TEXT("Spawn Attack"));

	if (NewProjectile) {
		// �߻�ü�� �ش� Turret�� Owner�� ���, �ش� Ŭ�������� ����� �� �ֵ��� �� 
		NewProjectile->SetOwner(this);
		NewProjectile->OwnerSetting();
		//UE_LOG(LogTemp, Warning, TEXT("NewProjectile : %s"), *NewProjectile->GetName());

		// �߻�ü�� ���� ��� Ÿ����
		NewProjectile->AttackTargeting();

	}

}

void ATurret_Base::Die()
{
	// Ÿ���� Mesh�� �� ������ ���� ����

	FVector CurrentLoc = MeshComp->GetComponentLocation();
	FVector DeathLoc = FVector(0, 0, -650);

	// ���� ���� (0.0�� ���� ��ġ, 1.0�� ��ǥ ��ġ)
	float LerpAlpha = 0.0001f; // ���ϴ� ������ ����

	// ������ ��ġ ���
	FVector LerpedLocation = FMath::Lerp(CurrentLoc, DeathLoc, LerpAlpha);

	// ������ ��ġ�� ������ ��ġ�� ����
	MeshComp->SetRelativeLocation(LerpedLocation);

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

