// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Garen.generated.h"

UCLASS()
class INABYSS_API AGaren : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGaren();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public: // 컴포넌트
	 
	// 상태 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateComponents")
	class UStateComponentBase* StateComp_Garen;

	// 스프링암 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArmComp;
	
	// 카메라 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComp;
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
//	class UInputMappingContext* DefaultMappingContext;



public:	// 키 입력
	// 마우스 오른쪽 입력
	void MouseRightClick();

public: // 이동기능
	void Move_Garen();

public: // 일반 공격기능
	void Attack_Normal_Garen();

public: // 스킬 기능
	void QSkill_Garen();
	void WSkill_Garen();
	void ESkill_Garen();
	void RSkill_Garen();

};
