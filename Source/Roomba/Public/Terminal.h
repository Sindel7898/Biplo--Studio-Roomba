// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terminal.generated.h"

class AGeneratorSwitch;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class ROOMBA_API ATerminal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerminal();

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GeneratorMesh;
	
	UPROPERTY(EditAnywhere)
	AGeneratorSwitch* GeneratorSwitch1;

	UPROPERTY(EditAnywhere)
	AGeneratorSwitch* GeneratorSwitch2;

	UPROPERTY(EditAnywhere)
	AGeneratorSwitch* GeneratorSwitch3;

	UPROPERTY(EditAnywhere)
	AGeneratorSwitch* GeneratorSwitch4;
	
	
	UPROPERTY()
	TArray<AGeneratorSwitch*> GeneratorSwitches;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnFadeComplete();
	int CountOfConnectedSwitches = 0;

};
