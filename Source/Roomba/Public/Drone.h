// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Drone.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class ROOMBA_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*// TODO DESCRIPTION
	FVector GetMovementVelocityMethod(float DeltaTime);*/

	// TODO DESCRIPTION
	float GetDirectionSpeedMethod(float DeltaTime, float InputAxisValue, float CurrentSpeed);

	 UPROPERTY(EditAnywhere,Category = Input)
	 UInputMappingContext* DefaultMappingContext;

	 UPROPERTY(EditAnywhere,Category = Input)
	 UInputAction* MoveAction;

	void OnInputChanged(const FInputActionValue& InputActionValue);

	void Move();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Root cube is the actual physics parts of the drone. This allows it to not be tied
	// into what mesh the artists are designing and their mass, scale, etc
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneComponents")
	UStaticMeshComponent* DroneRootCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneComponents")
	UStaticMeshComponent* DroneStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneComponents")
	USpringArmComponent* PrimarySpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneComponents")
	UCameraComponent* PrimaryCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementConfiguration")
	float MaxThrust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementConfiguration")
	float Acceleration;

	// TODO DESCRIPTION
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LiveStats");
	float CurrentForwardThrust;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LiveStats");
	float CurrentRightThrust;

	
private:
	FVector MoveResult;
	FVector2D MovementVector;

	
};
