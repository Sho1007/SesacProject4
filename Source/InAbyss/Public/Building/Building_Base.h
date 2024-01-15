// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/StateInterface.h"
#include "Building_Base.generated.h"

UENUM()
enum class EBuildingState : uint8
{
	NONE,
	IDLE,
	Attack,
	Destroy
};

UCLASS()
class INABYSS_API ABuilding_Base : public AActor, public IStateInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 콜리전 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CollisionComp;

	// 메쉬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* MeshComp;

public: // 다른 클래스의 액터 컴포넌트를 붙인 부분
	// 상태 컴포넌트 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateComponents")
	class UStateComponentBase* StateComponent_Building;
	
public:	// 상태 관련 설정
	EBuildingState GetBuildingState() const;

	UPROPERTY(VisibleInstanceOnly, Category = "BuildingState")
	EBuildingState BuildingState;

public: // 앞의 건물이 파괴되어야 자신을 파괴할 수 있도록 해야 함. 그 전까지는 무적상태
	UPROPERTY(EditInstanceOnly)
	TArray<ABuilding_Base*> PreBuilding;

	// 피격 가능 기능
	void ActivateHit();

public: // 인터페이스 가상함수 상속 부분
	//  피격시 호출될 함수
	virtual void Damaged() override;

	// 건축물이 파괴되었을 때 호출 = CurrentHealthPoiont_Building이 되었을 때 호출 
	virtual void Die() override;
	
};
