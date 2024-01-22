// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/InGamePlayerController.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
}