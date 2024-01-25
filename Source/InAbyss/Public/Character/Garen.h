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
	// 마우스 우클릭
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Mouse_Right_Action;

	// 마우스 좌클릭
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Mouse_Left_Action;

	// 키보드 Q
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Q_Action;

	// 키보드 W
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* W_Action;

	// 키보드 E
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* E_Action;

	// 키보드 R
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* R_Action;

	// 키보드 스페이스바
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SpaceBar_Action;

public: // 가렌 행동 상태 
	EGarenState GetGarenState() const;

	UPROPERTY(EditAnywhere)
	EGarenState GarenState;

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

	// 공격범위 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* AttackRange;



	// 나이아카라 이펙트 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* NSComp;

	// UI 위젯 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* HPBarComp;



public:	// 키 입력 함수
	// 마우스 오른쪽 입력
	void MouseRightClick(const FInputActionValue& value);
	void MouseRightClick_Triggered(const FInputActionValue& value);

	void MouseLeftClick(const FInputActionValue& value);

	void KeyBoard_Q(const FInputActionValue& value);
	void KeyBoard_W(const FInputActionValue& value);
	void KeyBoard_E(const FInputActionValue& value);
	void KeyBoard_R(const FInputActionValue& value);

	void KeyBoard_SpaceBar(const FInputActionValue& value);

public: // 이동기능
	FHitResult HitInfo;
	FVector CursorPlace;
	AActor* MouseHitActor;
	float Speed = 300;

	void Move_Garen();
	void Turn_Garen();

	FRotator CurrentRotation = FRotator(0);

	//FVector MovingPoint;

public: // 일반 공격기능
	void Attack_Normal_Garen();

	// 임시
	AActor* Target_Test;

	// 공격 대상 - 공격 대상을 구분하기 위함
	ACharacter* Target_Champion;
	class AMinionBase* Target_Minion;
	class ABuilding_Base* Target_Building;

	// 특정 스킬에서 다수의 대상을 공격하는 경우
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> Targets_Attack;





	// 공격범위
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDistance = 100.f;

	// 공격 감지 범위
	UPROPERTY(EditAnywhere, Category = "Attack")
	float DetectDistance = 100.f;

	void SettingTarget();



public: // 스킬 기능
	void Q_Skill_Garen();
	void W_Skill_Garen();
	void E_Skill_Garen();
	void R_Skill_Garen();
	void PassiveSkill_Garen();

	void R_Move();
	bool bIs_R_Move = false;

public: // 카메라 이동
	void CameraHold_Garen();

public: // 애니메이션
	UPROPERTY()
	class UGarenAnimInstance* GarenAnim;

public: // 인터페이스 가상함수 상속 부분
		virtual void Damaged() override;

		virtual void Die() override;
};
