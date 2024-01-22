// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(AbyssLog, Log, All);

#define NETMODE (GetNetMode() == ENetMode::NM_Standalone ? TEXT("Standalone") : \
	(GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : \
	(GetNetMode() == ENetMode::NM_ListenServer ? TEXT("ListenServer") : TEXT("DedicatedServer"))))

#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PRINTLOG(fmt, ...) UE_LOG(AbyssLog, Warning, TEXT("[%s]%s %s"), NETMODE, *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__))