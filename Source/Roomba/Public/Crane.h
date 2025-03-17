// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crane.generated.h"

class UStaticMeshComponent;

UCLASS()
class ROOMBA_API ACrane : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	ACrane();
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CraneRotator;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CraneGrabber;

	UPROPERTY(EditAnywhere)
	float RotationalSpeed = 1;
	
	UPROPERTY(EditAnywhere)
	float HorizontalSpeed = 1;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RotateCraneLeft(float RotationAmount);
	void RotateCraneRight(float RotationAmount);
	void MoveCraneUp(float MoveAmount);
	void MoveCraneDown(float MoveAmount);
};
