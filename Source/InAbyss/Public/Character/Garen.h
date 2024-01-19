// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "Garen.generated.h"

UENUM()
enum class EGarenState: uint8
{
	NONE,
	IDLE,
	MOVE,
	ATTACK,
	SKILL
};

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

public:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;

public:	// Mapping 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* IMC;

public: // InputAction
	// ���콺 ��Ŭ��
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Mouse_Right_Action;

	// ���콺 ��Ŭ��
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Mouse_Left_Action;

	// Ű���� Q
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Q_Action;

	// Ű���� W
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* W_Action;

	// Ű���� E
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* E_Action;

	// Ű���� R
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* R_Action;

	// Ű���� �����̽���
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SpaceBar_Action;

public: // ���� �ൿ ���� 
	EGarenState GetGarenState() const;

	UPROPERTY(EditAnywhere)
	EGarenState GarenState;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* AttackRange;

public:	// Ű �Է� �Լ�
	// ���콺 ������ �Է�
	void MouseRightClick(const FInputActionValue& value);

	void MouseLeftClick(const FInputActionValue& value);


public: // �̵����
	FHitResult HitInfo;
	FVector CursorPlace;
	AActor* MouseHitActor;

	void Move_Garen();
	void aaa();
	void bbb();

	FRotator CurrentRotation = FRotator(0);

	//FVector MovingPoint;

public: // �Ϲ� ���ݱ��
	void Attack_Normal_Garen();

	// �ӽ�
	AActor* Target_Test;

	// ���� ��� - ���� ����� �����ϱ� ����
	ACharacter* Target_Champion;
	class AMinionBase* Target_Minion;
	class ABuilding_Base* Target_Building;

public: // ��ų ���
	void QSkill_Garen();
	void WSkill_Garen();
	void ESkill_Garen();
	void RSkill_Garen();
	void PassiveSkill_Garen();

public: // ī�޶� �̵�
	void CameraHold_Garen();

};
