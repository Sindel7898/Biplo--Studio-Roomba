// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoombaGameMode.h"
#include "RoombaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARoombaGameMode::ARoombaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
