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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* AttackRange;

public:	// 키 입력 함수
	// 마우스 오른쪽 입력
	void MouseRightClick(const FInputActionValue& value);

	void MouseLeftClick(const FInputActionValue& value);


public: // 이동기능
	FHitResult HitInfo;
	FVector CursorPlace;
	AActor* MouseHitActor;

	void Move_Garen();
	void aaa();
	void bbb();

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

public: // 스킬 기능
	void QSkill_Garen();
	void WSkill_Garen();
	void ESkill_Garen();
	void RSkill_Garen();
	void PassiveSkill_Garen();

public: // 카메라 이동
	void CameraHold_Garen();

};
