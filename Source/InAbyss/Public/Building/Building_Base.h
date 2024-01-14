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
	// �ݸ��� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CollisionComp;

	// �޽� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* MeshComp;


public: // �ٸ� Ŭ������ ���� ������Ʈ�� ���� �κ�
	// ���� ������Ʈ 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateComponents")
	class UStateComponentBase* StateComponent_Building;
	
	/*
public:
	float CurrentHealthPoiont_Building;
	float MaxHealthPoiont_Building;
	*/

public: // �������̽� �����Լ� ��� �κ�
	//  �ǰݽ� ȣ��� �Լ�
	virtual void Damaged() override;

	// ���๰�� �ı��Ǿ��� �� ȣ�� = CurrentHealthPoiont_Building�� �Ǿ��� �� ȣ�� 
	virtual void Die() override;
	
};
