// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/StateInterface.h"
#include "Building_Base.generated.h"


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
	
	/*
public:
	float CurrentHealthPoiont_Building;
	float MaxHealthPoiont_Building;
	*/

public: // 인터페이스 가상함수 상속 부분
	//  피격시 호출될 함수
	virtual void Damaged() override;

	// 건축물이 파괴되었을 때 호출 = CurrentHealthPoiont_Building이 되었을 때 호출 
	virtual void Die() override;
	
};
