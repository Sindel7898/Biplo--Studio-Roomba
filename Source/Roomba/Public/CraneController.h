// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "CraneController.generated.h"

class UBoxComponent;
class ACrane;
class UInputAction;
class UInputMappingContext;

UCLASS()
class ROOMBA_API ACraneController : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraneController();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ControlDeskMesh;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* ControllerMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveCraneUp;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveCraneDown;
	
	UPROPERTY(EditAnywhere)
	UInputAction* MoveCraneLeft;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveCraneRight;

	/*UPROPERTY(EditAnywhere)
	UInputAction* SwitchBackToPlayer;*/
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveUp();
	void MoveDown();
	void RotateLeft();
	void RotateRight();
	//void SwitchToPlayer();
	

	UPROPERTY(EditAnywhere)
	ACrane* CraneRef;
	
};
