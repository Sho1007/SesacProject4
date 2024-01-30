// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/EzrealQ.h"

#include "Component/StateComponentBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AEzrealQ::AEzrealQ()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEzrealQ::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEzrealQ::OnComponentBeginOverlap);	
	}
}

void AEzrealQ::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Indicator == nullptr) return;
	
	if (UStateComponentBase* StateComponent = OtherActor->GetComponentByClass<UStateComponentBase>())
	{
		if (StateComponent->GetFactionType() != FactionType && StateComponent->GetObjectType() != EObjectType::BUILDING)
		{
			UStateComponentBase* IndicatorStateComponent = Indicator->GetComponentByClass<UStateComponentBase>();
			StateComponent->ApplyDamage(IndicatorStateComponent->GetAttackDamage(), IndicatorStateComponent->GetAbilityPower());
			Destroy();
		}
	}
}

// Called every frame
void AEzrealQ::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(GetActorForwardVector() * 50.f, false);
}

void AEzrealQ::Init(ACharacter* NewIndicator)
{
	Indicator = NewIndicator;
}
