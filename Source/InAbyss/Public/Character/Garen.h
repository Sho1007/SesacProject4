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


public: // ������Ʈ
	 
	// ���� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateComponents")
	class UStateComponentBase* StateComp_Garen;

	// �������� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArmComp;
	
	// ī�޶� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComp;
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
//	class UInputMappingContext* DefaultMappingContext;



public:	// Ű �Է�
	// ���콺 ������ �Է�
	void MouseRightClick();

public: // �̵����
	void Move_Garen();

public: // �Ϲ� ���ݱ��
	void Attack_Normal_Garen();

public: // ��ų ���
	void QSkill_Garen();
	void WSkill_Garen();
	void ESkill_Garen();
	void RSkill_Garen();

};
