// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Nexus.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>

ANexus::ANexus()
{
	// 넥서스의 Collision세팅
	CollisionComp->SetCapsuleRadius(350);


}
