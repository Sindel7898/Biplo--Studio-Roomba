// Copyright Epic Games, Inc. All Rights Reserved.

#include "Roomba/Public/RoombaGameMode.h"
#include "Roomba/Public/RoombaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARoombaGameMode::ARoombaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MovementPrototypeBP/MainDrone_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
