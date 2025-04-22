// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EnhancedInputComponent.h"
#include "RoombaSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ROOMBA_API URoombaSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CheckpointId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, bool> TriggeredPopups;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEnhancedInputComponent* CurrentEnhancedInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LumenEnabled;
};
