// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_Turret_Base.h"
#include "Components/SphereComponent.h"
#include "Building_Base.h"
#include "Components/SceneComponent.h"
#include "Projectile_Turret.h"

ABD_Turret_Base::ABD_Turret_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	// 포탑의 사정거리를 감지할 콜리전
	DetectCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCollisionComp"));
	DetectCollisionComp->SetupAttachment(RootComponent);
	DetectCollisionComp->SetRelativeScale3D(FVector(4.0f));
	DetectCollisionComp->SetRelativeLocation(FVector(0, 0, -20));

	// 공격 - 발사체를 스폰할 위치
	AttackStartPointComp = CreateDefaultSubobject<USceneComponent>(TEXT("AttackStartPointComp"));
	AttackStartPointComp->SetupAttachment(RootComponent);
	AttackStartPointComp->SetRelativeLocation(FVector(0, 0, 90));

	UE_LOG(LogTemp, Warning, TEXT("New Log========================================================"));

}

void ABD_Turret_Base::Tick(float DeltaTime)
{

	// 충돌 중인 액터가 있고, CurrentTarget이 없는 경우
	if (DetectTargets_Test.Num() > 0 && CurrentTarget == nullptr) {
		
		// 공격대상 재지정 함수 호출
		RetargetCurrentTarget();

	}

	// ----------------------------------------

	// 공격기능
	// 만약 공격대상이 지정되어 있다면
	if (CurrentTarget || TopPriorityTarget) {
		
		// 시간을 카운트 시작
		CurrentTIme += DeltaTime;

		// 2초가 지나면
		if (CurrentTIme >= TargetTIme) {

			CurrentTIme = 0.f;

			// 공격함수 호출 - 발사체 스폰
			Attact_SpawnProjectile();
		}

	}
	else
	{
		CurrentTIme = 0.f;
	}
	
	if (TopPriorityTarget) {
		// 디버그라인 말고 이펙트로 CurrentTarget을 가리키도록 해야 함
		DrawDebugLine(GetWorld(), AttackStartPointComp->GetComponentLocation(), TopPriorityTarget->GetActorLocation(), FColor::Red, false, -1, 0, 1.0f);
	}
	else if (CurrentTarget) {
		// 디버그라인 말고 이펙트로 CurrentTarget을 가리키도록 해야 함
		DrawDebugLine(GetWorld(), AttackStartPointComp->GetComponentLocation(), CurrentTarget->GetActorLocation(), FColor::Red, false, -1, 0, 1.0f);
	}

}

void ABD_Turret_Base::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// test ==================================================================================================

	// 포탑에 접근한 액터를 배열에 저장 -> 충돌한 액터를 배열에 저장 
	// + 감지된 액터가 적인 경우에만 저장하도록 해야 함*************************
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




		UE_LOG(LogTemp, Warning, TEXT("CurrentTarget : %s"), *CurrentTarget->GetName());

	}

	// test ==================================================================================================

	// 포탑의 콜리전과 충돌하는 액터를 배열에 저장한다. 
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



	// 나중에 조건 필요 - OtherActor가 챔피언 혹은 미니언인 경우에만 저장 => OtherActor를 if문으로 클래스를 검사한 뒤, 클래스에 맞는 배열에 저장할 것임
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










	// 공격타겟의 이탈의 경우 - CurrentTarget가 챔피언이고, 포탑의 범위에서 벗어난 경우
	if (CurrentTarget && CurrentTarget == OtherActor) {

		CurrentTarget = nullptr;




		//UE_LOG(LogTemp, Warning, TEXT("No CurrentTarget"));

		/*
		// 이 부분은 없어도 될 듯
		// 공격 타겟이 없어졌을 때, DetectTargets_Test의 크기가 1이상 이라면 우선순위에 따라 CurrentTarget을 재설정하는 함수를 호출한다.
		if (DetectTargets_Test.Num() > 0) {

			RetargetCurrentTarget();

		}
		*/
	}

	//UE_LOG(LogTemp, Warning, TEXT("EndOverlap-----------------------------------------------------"));

}

void ABD_Turret_Base::RetargetCurrentTarget()
{
	// 충돌하고 있는 유닛의 우선순위에 따라 CurrentTarget을 재지정한다. 

	// CurrentTarget이 있다면 함수 종료
	if (CurrentTarget) {
		return;
	}



	// test ==================================================================================================
	// 남아있는 배열이 있다면, 0번 인덱스를 공격 대상으로 함 
	if (DetectTargets_Test.Num() > 0) {
		CurrentTarget = DetectTargets_Test[0];



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

	UE_LOG(LogTemp, Warning, TEXT("CurrentTarget : %s"), *CurrentTarget->GetName());


}

void ABD_Turret_Base::Attact_SpawnProjectile()
{
	// 이거는 아닌 듯
	/*
	FHitResult hitInfo;
	FVector StartLocation = AttackStartPointComp->GetComponentLocation();
	FVector EndLocation = CurrentTarget->GetActorLocation();

	GetWorld()->LineTraceSingleByChannel(hitInfo, StartLocation, EndLocation, ECC_Visibility);
	*/
	
	
	// 발사체를 스폰해야 함 - 공격 자체는 발사체가 타겟을 따라가는 방식으로 이루어질 것
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


