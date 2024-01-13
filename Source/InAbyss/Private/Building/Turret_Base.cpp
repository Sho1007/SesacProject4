// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Turret_Base.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Building/Projectile_Turret.h"
#include "Component/StateComponentBase.h"
#include "Components/CapsuleComponent.h"

ATurret_Base::ATurret_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	// 포탑의 사정거리를 감지할 콜리전
	DetectCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCollisionComp"));
	DetectCollisionComp->SetupAttachment(RootComponent);
	DetectCollisionComp->SetSphereRadius(500);

	// 공격 - 발사체를 스폰할 위치
	AttackStartPointComp = CreateDefaultSubobject<USceneComponent>(TEXT("AttackStartPointComp"));
	AttackStartPointComp->SetupAttachment(RootComponent);
	AttackStartPointComp->SetRelativeLocation(FVector(50, 160, 590));

	//UE_LOG(LogTemp, Warning, TEXT("New Play Log========================================================"));

}

void ATurret_Base::BeginPlay()
{
	Super::BeginPlay();

	TurretState = ETurretState::IDLE;
	
}

void ATurret_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TurretState == ETurretState::Destroy) {

		if (MeshComp->GetRelativeLocation().Z >= FVector(0, 0, -650).Z) {

			MeshComp->SetRelativeLocation(MeshComp->GetRelativeLocation() - FVector(0,0,10));
		}

		return;
	}

	// --------------------------------------------------------------------------------

	// 최우선 대상이 있다면, 해당 대상을 공격타겟으로 저장
	if (TopPriorityTarget) {
		CurrentTarget = TopPriorityTarget;
	}

	// --------------------------------------------------------------------------------



	// Test ===================================================
	// CurrentTarget이 없고, 충돌 중인 액터가 있는 경우
	if (CurrentTarget == nullptr && DetectTargets_Test.Num() > 0 ) { 
	//  이후 조건 미니언, 챔피언 배열들 추가해서 수정 필요

		// 공격대상 재지정 함수 호출
		RetargetCurrentTarget();

	}
	// Test ===================================================




	// ----------------------------------------

	// 공격기능
	// 만약 공격대상이 지정되어 있다면
	if (CurrentTarget) {

		TurretState = ETurretState::ATTACK;
		
		// 시간을 카운트 시작
		CurrentTIme += DeltaTime;

		// 2초가 지나면
		if (CurrentTIme >= TargetTIme) {

			CurrentTIme = 0.f;

			// 공격함수 호출 - 발사체 스폰
			Attact_SpawnProjectile();
		}

	}
	else // 공격대상이 없으면 시간 초기화
	{
		TurretState = ETurretState::IDLE;

		CurrentTIme = 0.f;
	}

	// --------------------------------------------------------------------------------

	// 타겟팅 대상 표시 기능
	if (TopPriorityTarget) { // 최우선순위
		// 디버그라인 말고 이펙트로 CurrentTarget을 가리키도록 해야 함
		DrawDebugLine(GetWorld(), AttackStartPointComp->GetComponentLocation(), TopPriorityTarget->GetActorLocation(), FColor::Red, false, -1, 0, 1.0f);
	}
	else if (CurrentTarget) { // 우선순위 
		// 디버그라인 말고 이펙트로 CurrentTarget을 가리키도록 해야 함
		DrawDebugLine(GetWorld(), AttackStartPointComp->GetComponentLocation(), CurrentTarget->GetActorLocation(), FColor::Red, false, -1, 0, 1.0f);
	}

}

void ATurret_Base::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// test ==================================================================================================
	// 포탑에 접근한 액터를 배열에 저장 -> 충돌한 액터를 배열에 저장 

	// 임시 : 발사체는 배열에 저장하지 않음 - 이건 나중에 배열을 미니언, 챔피언만 인식하도록 하면 필요 없음
	if (OtherActor->IsA<AProjectile_Turret>()) { // 임시
		return;
	}

	// 범위에 들어온 대상을 공격대상 배열에 넣음
	DetectTargets_Test.Add(OtherActor);

	/* &&&&&&&&&&&&&&&&&로그
	// 배열의 인덱스 값을 출력하는 로그
	for (AActor* Actor : DetectTargets_Test)
	{
		if (Actor)
		{
			// AActor* 타입의 주소값을 출력하거나, 해당 액터의 이름 등을 출력할 수 있습니다.
			UE_LOG(LogTemp, Warning, TEXT("Detected Actor: %s"), *Actor->GetName());
		}
	}
	*/

	// CurrentTarget가 없는 경우, 배열의 0번 인덱스 = 가장 먼저 접근한 액터를 CurrentTarget에 저장 -> 공격대상으로 삼음
	if (CurrentTarget == nullptr) {

		CurrentTarget = DetectTargets_Test[0];

		Attact_SpawnProjectile(); // 타겟이 지정되면 바로 공격

		//UE_LOG(LogTemp, Warning, TEXT("CurrentTarget : %s"), *CurrentTarget->GetName());

	}

	// test ==================================================================================================





	// 포탑의 콜리전과 충돌하는 액터를 배열에 저장한다. 
	// + 감지된 액터가 적인 경우에만 저장하도록 해야 함*************************
	/*

	if (OtherActor->IsA<클래스 이름>()) {
		DetectTargets_SuperOrCanon.Add(OtherActor);

	}
	else if (OtherActor->IsA<클래스 이름>()) {
		DetectTargets_Warrior.Add(OtherActor);

	}
	else if (OtherActor->IsA<클래스 이름>()) {
		DetectTargets_Wizard.Add(OtherActor);

	}
	else if (OtherActor->IsA<클래스 이름>()) {
		DetectTargets_EnemyChampion.Add(OtherActor);

	}



	*/


	//UE_LOG(LogTemp, Warning, TEXT("BiginOverlap-----------------------------------------------------"));
}

void ATurret_Base::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	// => 이탈한 OtherActor의 클래스를 검사한 뒤, 클래스에 해당하는 배열에서 해당 액터를 제거해야 함

	// test ==================================================================================================

	// 포탑의 범위에서 벗어난 액터는 배열에서 제거 -> 충돌이 해제된 액터를 배열에서 제거한다. 
	DetectTargets_Test.Remove(OtherActor); // 사실상 기능으로는 이 코드만 필요함

	/*
	// 포탑에 접근 중인 액터가 남아있는지 검사하는 로그
	if (DetectTargets_Test.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("No Detect"));

	}
	else {
		for (AActor* Actor : DetectTargets_Test)
		{
			if (Actor)
			{
				// AActor* 타입의 주소값을 출력하거나, 해당 액터의 이름 등을 출력할 수 있습니다.
				UE_LOG(LogTemp, Warning, TEXT("Detected Actor: %s"), *Actor->GetName());
			}
		}
	}
	*/
	// test ==================================================================================================







	// 이탈 대상이 최우선 타겟이라면, 이를 공석으로 함
	if (TopPriorityTarget  && TopPriorityTarget == OtherActor) {
	
		TopPriorityTarget = nullptr;

	}

	// 공격타겟이 이탈한 경우 - CurrentTarget이 포탑의 범위에서 벗어난 경우
	if (CurrentTarget && CurrentTarget == OtherActor) {
		
		CurrentTarget = nullptr;

	}

	// 포탑의 범위에서 이탈한 액터를 배열에서 제거한다. - 제거할 때 인덱스는 자동으로 당겨서 정렬된다. 
	/*

	if (OtherActor->IsA<클래스 이름>()) {
		DetectTargets_SuperOrCanon.Remove(OtherActor);

	}
	else if (OtherActor->IsA<클래스 이름>()) {
		DetectTargets_Warrior.Remove(OtherActor);

	}
	else if (OtherActor->IsA<클래스 이름>()) {
		DetectTargets_Wizard.Remove(OtherActor);

	}
	else if (OtherActor->IsA<클래스 이름>()) {
		DetectTargets_EnemyChampion.Remove(OtherActor);

	}
	*/


}

ETurretState ATurret_Base::GetTurretState() const
{
	return ETurretState();
}

void ATurret_Base::RetargetCurrentTarget()
{
	// 본래 공격받던 타겟이 사라졌을 경우 호출하도록 해야 한다. - CurrentTarget이 nullptr인 경우
	// 충돌하고 있는 유닛의 우선순위에 따라 CurrentTarget을 재지정한다. 

	// 만약 최우선순위 대상 혹은 공격대상이 있다면 함수 종료
	if (TopPriorityTarget || CurrentTarget) {

		return;
	}



	// test ==================================================================================================
	// 남아있는 배열이 있다면, 0번 인덱스를 공격 대상으로 함 
	if (DetectTargets_Test.Num() > 0) {
		CurrentTarget = DetectTargets_Test[0];

		return;

		/*
		// 타겟을 출력하는 로그
		if (CurrentTarget) {
			UE_LOG(LogTemp, Warning, TEXT("CurrentTarget : %s"), *CurrentTarget->GetName());

		}
		else if (CurrentTarget == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("No CurrentTarget"));

		}
		*/
	}

	// test ==================================================================================================



	// 남아있는 배열이 있다면, 우선순위에 따라 해당 배열의 0번 인덱스를 공격 대상으로 함 
	// 1. 슈퍼/공성 미니언 ==== 2.전사 미니언 ==== 3. 마법사 미니언 ==== 4. 적 챔피언
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

	//UE_LOG(LogTemp, Warning, TEXT("CurrentTarget : %s"), *CurrentTarget->GetName());


}

void ATurret_Base::Attact_SpawnProjectile()
{

	// 발사체를 스폰해야 함 - 공격 자체는 발사체가 타겟을 따라가는 방식으로 이루어질 것
	AProjectile_Turret* NewProjectile = GetWorld()->SpawnActor<AProjectile_Turret>(ProjectileFactory, AttackStartPointComp->GetComponentTransform());

	//UE_LOG(LogTemp, Warning, TEXT("Spawn Attack"));

	if (NewProjectile) {
		// 발사체가 해당 Turret을 Owner로 삼고, 해당 클래스에서 사용할 수 있도록 함 
		NewProjectile->SetOwner(this);
		NewProjectile->OwnerSetting();
		//UE_LOG(LogTemp, Warning, TEXT("NewProjectile : %s"), *NewProjectile->GetName());

		// 발사체의 공격 대상 타겟팅
		NewProjectile->AttackTargeting();

	}

}

void ATurret_Base::TakeDamage_Turret()
{
	StateComponent_Building->ApplyDamage(10.f); // 임시 10 - 타격 대상의 공격력이 들어가야 할 듯
	
	// 실행 순서
		// TakeDamage_Turret() -> StateComponent_Building->ApplyDamage() -> OnRep_Health() -> Damaged() / Die()
}

void ATurret_Base::Damaged()
{
	// 포탑의 체력이 0보다 크면 - 호출할 거 없을 듯?


}

void ATurret_Base::Die()
{
	// 포탑의 체력이 0 이하이면
	
	// 모든 콜리전 NoCollision
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetectCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 공격 대상을 모두 제거
	if (TopPriorityTarget) {
		TopPriorityTarget = nullptr;
	}
	if (CurrentTarget) {
		CurrentTarget =nullptr;
	}

	// 임시 : 감지배열도 모두 null로 변경 
	if (DetectTargets_Test.Num() > 0) {
		DetectTargets_Test.Reset();

	}


	// 포탑 상태 Destroy로 변경
	TurretState = ETurretState::Destroy;
	
}

/*

공격

만약 TopPriorityTarget에 저장된 값이 있다면, 해당 액터를 공격한다.

그렇지 않고, CurrentTarget에 값이 있다면 해당 액터를 공격한다.

만약 위 두 조건이 없다면, 아무도 공격하지 않는다.



공격은 반드시 명중하고, 목표를 쫓아간다.

위에 지팡이 부근에서 일자로 라인이 그려지면서 타겟팅이 되고,
일정한 시간(2초)마다 발사체를 발사한다.

발사된 발사체는 목표를 쫓아서 날아가고,
발사체가 목표와 충돌하면 데미지를 입힌다.

*/

/*

플레이어 및 미니언을 감지

전제조건
포탑의 콜리전과 충돌 중인 액터를 감지한다.

1번 - 콜리전과 새로운 액터가 충돌할 때마다, 해당 액터를 DetectTargets 배열에 저장한다.
	1.1번 - 충돌한 순서대로 배열에 저장한다.
		->

2번 - 콜리전에서 이탈한 유닛이 있다면, 해당 유닛은 DetectTargets 배열에서 삭제한다.
		->


*/

/*

전제조건
포탑이 공격 가능한 액터는 공격시에 포탑의 콜리전과 충돌 중인 액터뿐이다.
		-> DetectTargets 배열의 크기가 1 이상인 경우에 호출할 수 있다.

특수조건(최우선순위)
0번 - 1번~3번 중 어떠한 경우이던지, 콜리전과 충돌중인 적 챔피언이 아군의 챔피언을 공격한 경우, 해당 적 챔피언을 공격 대상으로 한다.
		-> 이미 CurrentTarget에 저장된 값이 있더라도, 아군 챔피언 피격시 호출된다.
		-> 어떻게 공격/피격 여부를 알음????? -> 공격한 혹은 피해입은 챔피언이 알려줄 것 => 석현이 형이 해줄 것
		-> 아군 챔피언을 공격한 적 챔피언을 TopPriorityTarget에 저장한다.




일반조건(우선순위)
1번 - 가장 먼저 감지된 유닛을 공격 대상으로 삼고, 공격한다.
		-> CurrentTarget이 비어있는 경우, DetectTargets의 0번 인덱스의 유닛을 CurrentTarget에 저장한다.

2번 - CurrentTarget에 저장된 유닛의 사망 혹은 이탈의 경우, 새로운 공격 대상을 찾는다.
		-> 포탑이 공격을 마쳤을 때, CurrentTarget의 값이 없다면 3번을 호출한다.

3번 - 만약 이 때 이미 콜리전과 충돌하고 있는 액터가 있다면, 그 안에서 우선순위의 조건에 따라 공격 대상을 선정한다.
	3.1번 - 슈퍼/공성 미니언 -> 전사 미니언 -> 마법사 미니언 -> 적 챔피언
	3.2번 동일 우선순위라면, 먼저 감지된 유닛을 공격 대상으로 한다.
		-> CurrentTarget의 값이 없고, DetectTargets의 크기가 1이상인 경우에 호출할 수 있다.
		-> if문을 통해 우선순위에 해당하는 유닛을 찾는다.
		-> 배열에 들어있는 인덱스를 0번부터 검사한다.
		-> 만약 검사하는 인덱스의 유닛이 CurrentTarget에 저장된 유닛보다 우선순위가 높다면, 해당 인덱스의 유닛을 CurrentTarget에 저장한다.


*/

