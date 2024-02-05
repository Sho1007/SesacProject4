// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "Interface/StateInterface.h"
#include "CharacterBase.h"
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
class INABYSS_API AGaren : public ACharacterBase
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

	/*
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	class UGarenFSMComponent* FSMComponent;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	class UGarenSkillComponent* SkillComponent;
	*/

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	class UStateComponentBase* StateComponentBase;





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

	void ChangeState_Attack();

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

	// ����� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAudioComponent* ADComp;

	// ����� ���̽� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAudioComponent* VoiceComp;

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

	void SetTarget();

public: // ��ų ���
	UFUNCTION()
	void Q_Skill_Garen();
	UFUNCTION()
	void W_Skill_Garen();
	UFUNCTION()
	void E_Skill_Garen();
	UFUNCTION()
	void R_Skill_Garen();
	
	bool bIs_R_Move = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> NSFactory;

public: // �ִϸ��̼�
	UPROPERTY()
	class UGarenAnimInstance* GarenAnim;

public: 
	// ��ų ����Ʈ �迭
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	TArray<class UNiagaraSystem*> SkillEffect;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TArray<class USoundBase*> GR_Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TArray<class USoundBase*> GR_SkillSounds;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TArray<class USoundBase*> GR_SkillVoice;
//
//public: // �������̽� �����Լ� ��� �κ�
//		virtual void Damaged() override;
//
//		virtual void Die() override;

	virtual void Damaged() override;
	virtual void Die() override;
};
