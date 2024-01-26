// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "Interface/StateInterface.h"
#include "Garen.generated.h"

UENUM()
enum class EGarenState: uint8
{
	NONE,
	IDLE,
	MOVE,
	ATTACK,
	SKILL,
	DEAD
};

UCLASS()
class INABYSS_API AGaren : public ACharacter, public IStateInterface
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

	// ���ݹ��� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* AttackRange;



	// ���̾�ī�� ����Ʈ ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* NSComp;

	// UI ���� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* HPBarComp;



public:	// Ű �Է� �Լ�
	// ���콺 ������ �Է�
	void MouseRightClick(const FInputActionValue& value);
	void MouseRightClick_Triggered(const FInputActionValue& value);

	void MouseLeftClick(const FInputActionValue& value);

	void KeyBoard_Q(const FInputActionValue& value);
	void KeyBoard_W(const FInputActionValue& value);
	void KeyBoard_E(const FInputActionValue& value);
	void KeyBoard_R(const FInputActionValue& value);

	void KeyBoard_SpaceBar(const FInputActionValue& value);

public: // �̵����
	FHitResult HitInfo;
	FVector CursorPlace;
	AActor* MouseHitActor;
	float Speed = 300;

	void Move_Garen();
	void Turn_Garen();

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

	// Ư�� ��ų���� �ټ��� ����� �����ϴ� ���
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> Targets_Attack;





	// ���ݹ���
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDistance = 100.f;

	// ���� ���� ����
	UPROPERTY(EditAnywhere, Category = "Attack")
	float DetectDistance = 100.f;

	void SettingTarget();



public: // ��ų ���
	void Q_Skill_Garen();
	void W_Skill_Garen();
	void E_Skill_Garen();
	void R_Skill_Garen();
	void PassiveSkill_Garen();

	void R_Move();
	bool bIs_R_Move = false;

public: // ī�޶� �̵�
	void CameraHold_Garen();

public: // �ִϸ��̼�
	UPROPERTY()
	class UGarenAnimInstance* GarenAnim;

public: // �������̽� �����Լ� ��� �κ�
		virtual void Damaged() override;

		virtual void Die() override;
};
